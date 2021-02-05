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
#include"docker/private/adapter.array_access.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_callback.hpp"
#include"lock/atom_swap.hpp"
#include"macro/xis_nullptr.hpp"
#include"macro/xnew.hpp"
#include"memop/cast.hpp"
#include"memory/allocator.hpp"
#include"meta/has_cast.hpp"
#include"meta/has_constructor.hpp"
#include"meta/remove_ptr.hpp"
#include"meta/remove_ref.hpp"
#include"meta_seq/vlist.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_array{
    template<class item_t, uxx count>
    using pack = item_t[count];

    template<class final, class type, uxx count = 0, uxx ... rest>
    struct array_t;

    template<class final, class type, uxx count, uxx ... rest>
    inline auto configure(final *, type *, inc::vlist<count, rest...>){
        if constexpr (sizeof...(rest) == 0){
            return (pack<type, count> *)nullptr;
        }
        else{
            return (pack<array_t<final, type, rest...>, count> *)nullptr;
        }
    }

    template<class final, class type, uxx count, uxx ... rest>
    using items = inc::remove_ptr<
        decltype(
            configure(
                (final *)nullptr, 
                (type  *)nullptr, 
                (inc::vlist<count, rest...>{})
            )
        )
    >;

    /* 结构：静态数组
     * 参数：
     * - final 为接口结构类型
     * - type 为第一维度元素类型
     * - count 为第一维度元素个数
     * - rest 为更高维度元素个数
     */
    template<class final, class type, uxx count, uxx ... rest>
    xstruct(
        xtmpl(array_t, final, type, count, rest...),
        xprif(data, items<final, type, count, rest...>)
    )
        using item_t = inc::remove_ref<decltype(data[0])>;
    public:
        constexpr array_t() : data(){}
        constexpr array_t(array_t const &) = default;

        template<class ... args>
        constexpr array_t(item_t const & first, args const & ... list) : 
            data { first, ((item_t)list)... } {}

        template<class finalx>
        constexpr array_t(array_t<finalx, type, count, rest...> const & self) : 
            array_t((the_t &)(array_t<finalx, type, count, rest...> &)self) {
        }

        /* 函数：下标随机访问
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t & operator[] (uxx index) {
            return data[index];
        }

        /* 函数：下标随机访问（const 修饰）
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t const & operator[] (uxx index) const {
            return data[index];
        }

        /* 函数：获取数组元素的首地址 */
        operator item_t *() {
            return data;
        }

        /* 函数：获取数组元素的首地址（const 修饰） */
        operator item_t const *() const {
            return data;
        }

        /* 属性：当前维度数组元素的个数 */
        xpubgetx(length, uxx){
            return count;
        }
    $

    static inline uxx   empty_array     = 0;

    template<class final, class type>
    xstruct(
        xspec(array_t, final, type),
        xpubb(inc::disable_copy),
        xprif(data, type *)
    )
        using item_t                = type;
        using item_initial_invoke   = inc::icallback<void(item_t * item_ptr)>;
        using item_initial_invokex  = inc::icallback<void(uxx i, item_t * item_ptr)>;

        constexpr array_t(decltype(nullptr) = nullptr) : 
            data(empty_array_ptr()){
        }

        template<class initial_invoke>
        requires(
            inc::has_cast<item_initial_invoke , initial_invoke> or 
            inc::has_cast<item_initial_invokex, initial_invoke>
        )
        array_t(::length capacity, inc::ialloc<void> alloc, initial_invoke const & initial) : 
            data(create(capacity, alloc)){

            for(uxx i = 0, len = the.length(); i < len; i++){
                if constexpr (inc::has_cast<item_initial_invoke, initial_invoke>){
                    initial(data + i);
                }
                else{
                    initial(i, data + i);
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

        template<class ... args>
        requires(inc::has_constructor<item_t, void(args const & ...)> == true)
        array_t(::length capacity, inc::ialloc<void> alloc, args const & ... item_initial_args):
            // 编译器差异导致必须让 concept 和 bool true 比较才可以被 requires 接受
            array_t(capacity, alloc, [&](item_t * item_ptr){
                xnew(item_ptr) item_t(item_initial_args...);
            }){
        }

        template<class ... args>
        requires(inc::has_constructor<item_t, void(args const & ...)> == true)
        array_t(::length capacity, args const & ... item_initial_args){
            array_t(capacity, [&](item_t * item_ptr){
                xnew(item_ptr) item_t(item_initial_args...);
            });
        }

        array_t(::length capacity):
            array_t(capacity, [](item_t * item_ptr){
                xnew(item_ptr) item_t();
            }){}
        
        template<class ... args>
        requires(... and inc::has_cast<item_t, args>)
        array_t(item_t const & first, args const & ... rest) : 
            data(
                create(::length{1 + sizeof...(args)}, inc::default_alloc<void>)
            ){

            struct item_ref{
                item_t const & value;
                item_ref(item_t const & value) : value(value){}
            } items[] = {first, rest...};

            for(uxx i = 0, len = the.length(); i < len; i++){
                xnew(the.data + i) item_t(items[i].value);
            }
        }
    protected:
        ~array_t(){
            if (the == nullptr){
                return;
            }

            auto   old_ptr  = empty_array_ptr();
            auto & old      = inc::cast<the_t>(old_ptr);
            old.data        = inc::atom_swap(& data, old.data);

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
            return origin(data);
        }

        uxxp header() const {
            return uxxp(uxx(data) & ~uxx(1)) - 1;
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
                return (uxx(data) & 1) != 0;
            }
            xw{ 
                data = (item_t *)(uxx(origin()) | uxx(value));
            }

        xpubgetx(length, uxx){
            return header()[0];
        }
    $

    template<class final, class type, uxx count, uxx ... rest>
    using array = inc::adapter_array_access<
        array_t<final, type, count, rest...>
    >;
}

#endif

#include"math/index_system.hpp"

#undef  xusing_docker_array
#define xusing_docker_array     ::mixc::docker_array
