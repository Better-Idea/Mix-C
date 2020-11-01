#ifndef xpack_meta_seq_vnull
#define xpack_meta_seq_vnull
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vnull::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vnull{
    struct vnull_t{};
    constexpr vnull_t vnull{};
}

#endif

xexport(mixc::meta_seq_vnull::vnull)