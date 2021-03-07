#ifndef xpack_meta_seq_tselector_key
#define xpack_meta_seq_tselector_key
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tselector_key::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/private/xgen.tselector.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tselector_key{
    xmake_selector(key);
}

#endif

xexport(mixc::meta_seq_tselector_key::tselector_key)