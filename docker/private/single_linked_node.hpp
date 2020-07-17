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
    inline void foreach(
        single_linked_node<item_t> *                         node, 
        inc::iteratorx<single_linked_node<item_t> *> const & itr){
        for(uxx i = 0; node != nullptr; node = node->next, i++){
            if (itr(i, node) == loop_t::finish){
                break;
            }
        }
    }

    template<class item_t>
    inline void foreach(single_linked_node<item_t> * node, inc::iteratorx<item_t &> const & itr){
        foreach(node, [&](uxx index, single_linked_node<item_t> * cur){
            return itr(index, cur);
        });
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_single_linked_node::origin;
}
