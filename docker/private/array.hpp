/* 模块：array
 * 类型：基础结构
 * 功能：提供静态的一维或多维数组
 * 用法：
 * TODO======================================================================================
 */

#ifndef xpack_docker_array
#define xpack_docker_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_array::inc
#include"concurrency/lock/atom_swap.hpp"
#include"docker/private/adapter.array_access.hpp"
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_callback.hpp"
#include"interface/ranger.hpp"
#include"macro/xis_nullptr.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"macro/xstruct.hpp"
#include"memop/cast.hpp"
#include"meta/has_cast.hpp"
#include"meta/has_constructor.hpp"
#include"meta/remove_ptr.hpp"
#include"meta/remove_ref.hpp"
#include"meta_seq/vlist.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_array{
    template<class item_t, uxx count_v>
    using pack = item_t[count_v];

    template<class final_t, class type_t, uxx count_v = 0, uxx ... rest_v>
    struct array_t;

    template<class final_t, class type_t, uxx count_v, uxx ... rest_v>
    inline auto configure(final_t *, type_t *, inc::vlist<count_v, rest_v...>){
        if constexpr (sizeof...(rest_v) == 0){
            return (pack<type_t, count_v> *)nullptr;
        }
        else{
            return (pack<array_t<final_t, type_t, rest_v...>, count_v> *)nullptr;
        }
    }

    template<class final_t, class type_t, uxx count_v, uxx ... rest_v>
    using items = inc::remove_ptr<
        decltype(
            configure(
                (final_t *)nullptr, 
                (type_t  *)nullptr, 
                (inc::vlist<count_v, rest_v...>{})
            )
        )
    >;

    /* 结构：静态数组
     * 参数：
     * - final_t 为接口结构类型
     * - type_t 为第一维度元素类型
     * - count_v 为第一维度元素个数
     * - rest_v 为更高维度元素个数
     */
    template<class final_t, class type_t, uxx count_v, uxx ... rest_v>
    xstruct(
        xtmpl(array_t, final_t, type_t, count_v, rest_v...),
        xprif(m_items, items<final_t, type_t, count_v, rest_v...>)
    )
    public:
        using item_t = inc::remove_ref<decltype(the_t::m_items[0])>;

        constexpr array_t(array_t const &) = default;

        template<class ... args_t>
        requires(... and inc::has_cast<item_t, args_t>)
        constexpr array_t(args_t const & ... list) : 
            // (item_t &)(args_t &) 和指针的转换类似，会导致不正确的行为
            // m_items { ((item_t &)(args_t &)list)... } {}
            m_items { ((args_t &)list)... } {}

        template<class finalx_t >
        constexpr array_t(array_t<finalx_t, type_t, count_v, rest_v...> const & self) : 
            array_t((the_t &)(array_t<finalx_t, type_t, count_v, rest_v...> &)self) {
        }

        /* 函数：下标随机访问
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t & operator[] (uxx index) {
            return m_items[index];
        }

        /* 函数：下标随机访问（const 修饰）
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t const & operator[] (uxx index) const {
            return m_items[index];
        }

        /* 函数：获取数组元素的首地址 */
        operator item_t *() {
            return m_items;
        }

        /* 函数：获取数组元素的首地址（const 修饰） */
        operator item_t const *() const {
            return m_items;
        }

        /* 属性：当前维度数组元素的个数 */
        xpubgetx(length, uxx){
            return count_v;
        }

    public:
        xranger(item_t)
    $

    inline uxx   empty_array     = 0;

    template<class final_t, class type_t>
    xstruct(
        xspec(array_t, final_t, type_t),
        xpubb(inc::disable_copy),
        xprif(m_items, type_t *)
    )
        using item_t                = type_t;
        using item_initial_invoke   = inc::icallback<void(item_t * item_ptr)>;
        using item_initial_invokex  = inc::icallback<void(uxx i, item_t * item_ptr)>;

        constexpr array_t(decltype(nullptr) = nullptr) : 
            m_items(empty_array_ptr()){
        }

        template<class initial_invoke>
        requires(
            inc::has_cast<item_initial_invoke , initial_invoke> or 
            inc::has_cast<item_initial_invokex, initial_invoke>
        )
        array_t(::length capacity, inc::ialloc<void> alloc, initial_invoke const & initial) : 
            m_items(create(capacity, alloc)){

            for(uxx i = 0, len = the.length(); i < len; i++){
                if constexpr (inc::has_cast<item_initial_invoke, initial_invoke>){
                    initial(m_items + i);
                }
                else{
                    initial(i, m_items + i);
                }
            }
        }

        template<class initial_invoke>
        requires(
            inc::has_cast<item_initial_invoke , initial_invoke> or 
            inc::has_cast<item_initial_invokex, initial_invoke>
        )
        array_t(::length capacity, initial_invoke const & initial):
            array_t(capacity, inc::default_alloc<void>, initial){
            the.need_free(true);
        }

        template<class ... args_t>
        requires(inc::has_constructor<item_t, void(args_t const & ...)> == true)
        array_t(::length capacity, inc::ialloc<void> alloc, args_t const & ... item_initial_args):
            // 编译器差异导致必须让 concept 和 bool true 比较才可以被 requires 接受
            array_t(capacity, alloc, [&](item_t * item_ptr){
                xnew(item_ptr) item_t(item_initial_args...);
            }){
        }

        template<class ... args_t>
        requires(inc::has_constructor<item_t, void(args_t const & ...)> == true)
        array_t(::length capacity, args_t const & ... item_initial_args){
            array_t(capacity, [&](item_t * item_ptr){
                xnew(item_ptr) item_t(item_initial_args...);
            });
        }

        array_t(::length capacity):
            array_t(capacity, [](item_t * item_ptr){
                xnew(item_ptr) item_t();
            }){}

        template<class ... args_t>
        requires(... and inc::has_cast<item_t, args_t>)
        array_t(args_t const & ... list) : 
            m_items(
                create(::length{ sizeof...(args_t) }, inc::default_alloc<void>)
            ){

            struct item_ref{
                item_t const & value;
                item_ref(item_t const & value) : value(value){}
            } items[] = { list... };

            for(uxx i = 0, len = the.length(); i < len; i++){
                xnew(the.m_items + i) item_t(items[i].value);
            }
        }
    protected:
        ~array_t(){
            if (the == nullptr){
                return;
            }

            auto   old_ptr  = empty_array_ptr();
            auto & old      = inc::cast<the_t>(old_ptr);
            old.m_items        = inc::atom_swap(& m_items, old.m_items);

            if (not old.need_free()){
                return;
            }

            inc::free(
                old.header(), inc::memory_size{
                    sizeof(uxx) + old.length() * sizeof(item_t)
                }
            );
        }

        static item_t * create(uxx capacity, inc::ialloc<void> alloc){
            using itemp     = item_t *;
            auto    mem     = alloc(sizeof(uxx) + capacity * sizeof(item_t));
            auto &  len     = uxxp(mem)[0];
            auto    data    = itemp(uxxp(mem) + 1);
            len             = capacity;
            return data;
        }

        static item_t * empty_array_ptr(){
            return (item_t *)(& empty_array + 1);
        }

        static item_t * origin(voidp ptr){
            return (item_t *)(uxx(ptr) & ~uxx(1));
        }

        item_t * origin() const {
            return origin(m_items);
        }

        uxxp header() const {
            return uxxp(uxx(m_items) & ~uxx(1)) - 1;
        }
    public:
        /* 函数：下标随机访问
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t & operator[] (uxx index) {
            return origin()[index];
        }

        /* 函数：下标随机访问（const 修饰）
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t const & operator[] (uxx index) const {
            return origin()[index];
        }

        /* 函数：获取数组元素的首地址 */
        operator item_t *() {
            return origin();
        }

        /* 函数：获取数组元素的首地址（const 修饰） */
        operator item_t const *() const {
            return origin();
        }

        xis_nullptr(
            origin() == empty_array_ptr()
        )

        xpriget_prisetx(need_free, bool)
            xr{ 
                return (uxx(m_items) & 1) != 0;
            }
            xw{ 
                m_items = (item_t *)(uxx(origin()) | uxx(value));
            }

        xpubgetx(length, uxx){
            return header()[0];
        }

    public:
        xranger(item_t)
    $

    template<class final_t, class type_t, uxx count_v, uxx ... rest_v>
    using array = inc::adapter_array_access<
        array_t<final_t, type_t, count_v, rest_v...>
    >;
}

#endif

#include"math/index_system.hpp"

#undef  xusing_docker_array
#define xusing_docker_array     ::mixc::docker_array
