#ifndef xpack_gc_private_token
#define xpack_gc_private_token
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_token::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_fetch_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"define/base_type.hpp"
#include"dumb/struct_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_token::origin{
    constexpr uxx shift_to_get_owners       = (0);
    constexpr uxx step                      = (uxx(1) << shift_to_get_owners);
    constexpr uxx addition_bits             = (3);
    constexpr uxx visited_bits              = (sizeof(uxx) * 8 - addition_bits);
    constexpr uxx mask_to_get_addtion_bits  = (uxx(1) << addition_bits) - 1;

    xstruct(
        xname(token),
        xprof(m_record, uxx),
        xpubc(m_can_arrive_root, 1, volatile uxx),
        xpubc(m_under_release, 1, volatile uxx),
        xpubc(m_in_queue, 1, volatile uxx), // 使用 volvatle 限制内存必须写入，在 g++-10 中， -O2 似乎优化掉了这个
        xpubc(m_visited, visited_bits, uxx)
    )
        token(uxx) : 
            m_record(step),
            m_can_arrive_root(0),
            m_under_release(0),
            m_in_queue(0),
            m_visited(0){
            inc::atom_store(xref(m_record), step);
            inc::atom_store(xref(m_record) + 1, 0);
        }

        constexpr uxx  this_length() const { return uxx(0); }
        constexpr void this_length(uxx) const { }

    private:
        static inline uxx   g_base = 0;
        static inline uxx   g_top = 0;

        static uxx ajust_top(){
            // 高 3bit 不使用，避免 new_visited() 给 m_visited 赋值时溢出成 0
            auto value  = g_top & (uxx(-1) >> addition_bits); 
            return value;
        }
    public:
        xstruct(
            xname(free_node), // 该结构内存布局和 token 保持一致
            xprif(m_prev, free_node *),
            xpric(m_reserved, addition_bits, uxx),
            xpric(m_bytes, visited_bits, uxx)
        )
            using final_t       = free_node;
            xpubget_pubset(prev);
            xpubget_pubset(bytes);
        $;

        using free_nodep = free_node *;

        static void new_term(){
            // TODO:处理溢出的情况
            g_top      += g_top ? 1 : 2;
            g_top       = ajust_top();
            g_base      = g_top;
        }

        void visit_root(){
            m_visited   = g_base;
        }

        uxx new_visited(){
            if (not is_visited()){
                m_visited  = g_base;
            }

            // TODO:处理溢出的情况
            m_visited  += 1; 
            g_top      += g_top < m_visited;
            g_top       = ajust_top();
            return visited();
        }

        uxx visited(){
            return m_visited - g_base;
        }

        bool is_visited(){
            return m_visited >= g_base;
        }

        void can_arrive_root(bool value){
            m_can_arrive_root = value;
        }

        bool can_arrive_root(){
            return is_visited() and m_can_arrive_root;
        }

        bool under_release() {
            return m_under_release;
        }

        void under_release(bool value) {
            m_under_release = value;
        }

        bool in_gc_queue() const {
            return m_in_queue;
        }

        void in_gc_queue(bool value) {
            m_in_queue      = value;
        }

        uxx owners() const {
            return inc::atom_load(& m_record) >> shift_to_get_owners;
        }

        uxx owners_increase() const {
            return inc::atom_add(& m_record, step) >> shift_to_get_owners;
        }

        uxx owners_decrease() const {
            return inc::atom_sub(& m_record, step) >> shift_to_get_owners;
        }
    $

    xstruct(
        xname(token_plus),
        xpubb(token),
        xpubf(m_length, uxx)
    )
        token_plus(uxx length) : 
            token(0), m_length(length) { }

        uxx this_length() const {
            return m_length;
        }

        void this_length(uxx value) const {
            m_length = value;
        }
    $

    template<class type_t, class attribute_t, class addition_t>
    struct token_mix;

    template<class type_t, class addition_t>
    xstruct(
        xspec(token_mix, type_t, void, addition_t),
        xpubb(addition_t)
    )
        using base_t = addition_t;
        using base_t::base_t;

        token_mix(token_mix const &)        = delete;
        void operator=(token_mix const &)   = delete;

        type_t * item_ptr(uxx index){
            if constexpr (inc::is_same<void, type_t>){
                return nullptr;
            }
            else{
                return ((type_t *)(this + 1)) + index;
            }
        }

        ~token_mix() {
            if constexpr (not inc::is_same<void, type_t>){
                for(uxx i = 0; i < addition_t::this_length(); i++) {
                    item_ptr(i)->~type_t();
                }
            }
        }
    $

    template<class type_t, class attribute_t, class addition_t>
    xstruct(
        xtmpl(token_mix, type_t, attribute_t, addition_t),
        xpubb(token_mix<type_t, void, addition_t>), 
        xpubb(inc::struct_type<attribute_t>)
    )
        using base_t = token_mix<type_t, void, addition_t>;

        template<class ... args_t>
        token_mix(uxx length, args_t const & ... list) : 
            base_t(length), inc::struct_type<attribute_t>(list...) {}

        attribute_t * attribute_ptr() const {
            return (attribute_t *)(inc::struct_type<attribute_t> *)this;
        }
    $
}

#endif

xexport_space(mixc::gc_private_token::origin)