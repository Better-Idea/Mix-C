#ifndef xpack_docker_single_linked_node
#define xpack_docker_single_linked_node
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_single_linked_node
#include"dumb/struct_t.hpp"
#include"interface/iterator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_single_linked_node::origin {
    template<class item_t>
    xstruct(
        xtmpl(single_linked_node, item_t),
        xpubb(inc::struct_t<item_t>),
        xpubf(next, single_linked_node<item_t> *)
    )
        template<class ... args>
        single_linked_node(args const & ... list) :
            inc::struct_t<item_t>(list...), next(nullptr){
        }
    $

    template<class item_t>
    inline void foreach(single_linked_node<item_t> * node, inc::iteratorx<item_t &> const & itr){
        for(; node != nullptr; node = node->next){
            if (itr(node[0]) == loop_t::finish){
                break;
            }
        }
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_single_linked_node::origin;
}
