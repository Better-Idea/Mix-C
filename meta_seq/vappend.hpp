#ifndef xpack_meta_seq_vappend
#define xpack_meta_seq_vappend
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vappend::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/vlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vappend{
    using namespace inc;

    template<class vlist, auto item> struct vappend;
    template<auto item, auto ... args>
    struct vappend<vlist<args...>, item>{
        using new_list = vlist<args..., item>;
    };
}

#endif

xexport(mixc::meta_seq_vappend::vappend)