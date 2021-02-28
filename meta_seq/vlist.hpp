#ifndef xpack_meta_seq_vlist
#define xpack_meta_seq_vlist
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vlist::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vlist{
    template<auto ... vals_v>
    struct vlist{
        using type_list = inc::tlist<decltype(vals_v)...>;
        static constexpr uxx length = sizeof...(vals_v);
    };
}

#endif

xexport(mixc::meta_seq_vlist::vlist)