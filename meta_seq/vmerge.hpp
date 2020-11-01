#ifndef xpack_meta_seq_vmerge
#define xpack_meta_seq_vmerge
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vmerge::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/vlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vmerge{
    using namespace inc;

    template<class list_a, class list_b> struct vmerge;
    template<auto ... values_a, auto ... values_b> 
    struct vmerge<vlist<values_a ...>, vlist<values_b ...>>{
        using new_list = vlist<values_a ..., values_b ...>;
    };
}

#endif

xexport(mixc::meta_seq_vmerge::vmerge)