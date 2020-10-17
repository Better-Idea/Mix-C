#ifndef xpack_docker_single_linked_node
#define xpack_docker_single_linked_node
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_single_linked_node
#include"dumb/struct_type.hpp"
#include"interface/iterator.hpp"
#include"lock/atom_swap.hpp"
#include"lock/mutex.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_single_linked_node::origin {
    template<class item_t>
    xstruct(
        xtmpl(single_linked_node, item_t),
        xpubb(inc::struct_type<item_t>),
        xpubf(next, single_linked_node<item_t> *)
    )
        template<class ... args>
        single_linked_node(args const & ... list) :
            inc::struct_type<item_t>(list...), next(nullptr){
        }
    $
}

namespace mixc::docker_single_linked_node{
    template<class item_t, class barrier_t>
    xstruct(
        xtmpl(meta   , item_t, barrier_t),
        xpubf(ptop   , mutable origin::single_linked_node<item_t> *),
        xprif(barrier, barrier_t)
    )
        template<auto opr, class callback>
        void lock(callback const & call) const {
            barrier.template lock<opr>(call);
        }

        enum : uxx{
            mask = uxx(-1), // 不屏蔽任何位
        };
    $

    // 默认使用 base_t::ptop 的第 0 位作为互斥位
    template<class item_t>
    xstruct(
        xspec(meta, item_t, void),
        xpubf(ptop, mutable origin::single_linked_node<item_t> *)
    )
        template<auto/*dummy*/, class callback>
        void lock(callback const & call) const {
            inc::mutex::lock(xref ptop, 0, call);
        }

        enum : uxx{
            mask = uxx(-2), // 屏蔽第 0 位
        };
    $

    namespace origin{
        template<class item_t, class barrier_t>
        xstruct(
            xtmpl(node_field, item_t, barrier_t),
            xpubb(meta<item_t, barrier_t>)
        )
            using nodep  = origin::single_linked_node<item_t> *;
            using base_t = meta<item_t, barrier_t>;

            node_field(){
                base_t::ptop = nullptr;
            }

            static nodep origin(nodep node){
                return nodep(uxx(node) & base_t::mask); 
            }

            nodep top() const {
                return origin(base_t::ptop);
            }

            nodep swap_top(nodep value) const {
                nodep masked_top = inc::atom_swap<nodep>(xref base_t::ptop, masked(value));
                return origin(masked_top);
            }

            static nodep masked(nodep node) {
                return nodep(uxx(node) | ~base_t::mask);
            }
        $
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_single_linked_node::origin;
}
