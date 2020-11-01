#ifndef xpack_meta_seq_tpush
#define xpack_meta_seq_tpush
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tpush::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tappend.hpp"
#pragma pop_macro("xuser")
#endif

xexport(mixc::meta_seq_tpush::tpush)