#ifndef xpack_docker_single_linked_node
#define xpack_docker_single_linked_node
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_single_linked_node::inc
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/lock/builtin_lock.hpp"
#include"dumb/struct_type.hpp"
#include"meta/is_attached_lock.hpp"
#include"meta/is_same.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_single_linked_node::origin {
    template<class item_t>
    xstruct(
        xtmpl(single_linked_node, item_t),
        xpubb(inc::struct_type<item_t>),
        xpubf(next, single_linked_node<item_t> *)
    )
        template<class ... args_t>
        single_linked_node(args_t const & ... list) :
            inc::struct_type<item_t>(list...), next(nullptr){
        }
    $

    template<class item_t, inc::is_attached_lock lock_t = inc::lock_free>
    xstruct(
        xtmpl(single_linked_node_ptr, item_t, lock_t),
        xprib(lock_t),
        xprif(ptop,       single_linked_node<item_t> *)
    )
    public:
        using node_t    = single_linked_node<item_t>;
        using nodep     = node_t *;
        using base_t    = lock_t;
    private:
        enum{
            mask        = inc::is_same<lock_t, inc::with_lwmutex> ? uxx(-2) : uxx(-1), // 屏蔽第 0位 : 不屏蔽任何位
        };

        static nodep origin(nodep single_linked_nodex){
            return nodep(uxx(single_linked_nodex) & mask); 
        }

        static nodep masked(nodep single_linked_nodex) {
            return nodep(uxx(single_linked_nodex) | ~mask);
        }
    public:
        single_linked_node_ptr() : 
            ptop(nullptr){
        }

        template<auto operation, class callback>
        void lock(callback const & call) const {
            base_t::template lock<operation>(this, call);
        }

        nodep top() const {
            return origin(ptop);
        }

        nodep swap_top(nodep value) const {
            nodep masked_top = inc::atom_swap<nodep>(xref ptop, masked(value));
            return origin(masked_top);
        }
    $
}

#endif

xexport_space(mixc::docker_single_linked_node::origin)