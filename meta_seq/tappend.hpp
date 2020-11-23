#ifndef xpack_meta_seq_tappend
#define xpack_meta_seq_tappend
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tappend::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tappend{
    using namespace inc;

    template<class tlist, class item_t> struct tappend;
    template<class item_t, class ... args>
    struct tappend<tlist<args...>, item_t>{
        using new_list = tlist<args..., item_t>;
    };
}

#endif

xexport(mixc::meta_seq_tappend::tappend)