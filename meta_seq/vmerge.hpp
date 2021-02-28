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

    template<class vlist0_t, class vlist1_t> struct vmerge;
    template<auto ... vals0_v, auto ... vals1_v> 
    struct vmerge<vlist<vals0_v ...>, vlist<vals1_v ...>>{
        using new_list = vlist<vals0_v ..., vals1_v ...>;
    };
}

#endif

xexport(mixc::meta_seq_vmerge::vmerge)