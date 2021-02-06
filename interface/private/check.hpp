#ifndef xpack_interface_private_check
#define xpack_interface_private_check
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_private_check::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_private_check::origin{
    template<class seq_t>
    concept check_initializer_list = requires(seq_t list, uxx size, void const * ptr){
        size    = list.size();
        ptr     = list.begin();
    };

    template<class seq_t, class item_t>
    concept check_initializer_listx = requires(seq_t list, uxx size, item_t const * ptr){
        size    = list.size();
        ptr     = list.begin();
    };

    template<class seq_t>
    concept check_array  = requires(seq_t list, void const * ptr){
        ptr     = list;
    };

    template<class seq_t, class item_t>
    concept check_arrayx = requires(seq_t list, item_t const * ptr){
        ptr     = list;
    };

    template<class seq_t>
    concept check_indexable = requires(seq_t list, inc::item_origin_of<seq_t> * ptr, uxx index){
        ptr     = & list[index];
    };

    template<class seq_t, class item_t>
    concept check_indexablex = requires(seq_t list, item_t * ptr, uxx index){
        ptr = &list[index];
    };

    template<class seq_t>
    concept check_length = requires(seq_t list, uxx length){
        length  = list.length();
    };
}

#endif

xexport_space(mixc::interface_private_check::origin)