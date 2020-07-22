#ifndef xpack_docker_single_linked_node
#define xpack_docker_single_linked_node
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_single_linked_node
#include"dumb/struct_type.hpp"
#include"interface/iterator.hpp"
#include"lock/mutex.hpp"
#include"meta_ctr/cif.hpp"
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
        void lock(callback const & call){
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
        template<auto opr, class callback>
        void lock(callback const & call){
            inc::mutex::lock(xref ptop, 0, call);
        }

        enum : uxx{
            mask = uxx(-2), // 屏蔽第 0 位
        };
    $

    namespace origin{
        template<class item_t, class barrier_t, bool with_lock>
        using node_field = meta<item_t, inc::cif<with_lock, barrier_t, void>>;
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_single_linked_node::origin;
}
