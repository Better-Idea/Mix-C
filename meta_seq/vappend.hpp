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

    template<class vlist_t, auto item_t> struct vappend;
    template<auto item_t, auto ... args_t>
    struct vappend<vlist<args_t...>, item_t>{
        using new_list = vlist<args_t..., item_t>;
    };
}

#endif

xexport(mixc::meta_seq_vappend::vappend)