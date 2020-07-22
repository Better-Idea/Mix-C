#ifndef xpack_docker_single_linked_node
#define xpack_docker_single_linked_node
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_single_linked_node
#include"dumb/struct_type.hpp"
#include"interface/iterator.hpp"
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

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_single_linked_node::origin;
}
