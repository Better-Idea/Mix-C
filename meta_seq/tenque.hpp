#ifndef xpack_meta_seq_tenque
#define xpack_meta_seq_tenque
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tenque::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tappend.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tenque{
    template<class tlist, class item> 
    using tenque = inc::tappend<tlist, item>;
}

#endif

xexport(mixc::meta_seq_tenque::tenque)