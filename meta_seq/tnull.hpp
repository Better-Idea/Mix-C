#ifndef xpack_meta_seq_tnull
#define xpack_meta_seq_tnull
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tnull::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tnull{
    struct tnull{};
}

#endif

xexport(mixc::meta_seq_tnull::tnull)