#ifndef xpack_meta_seq_tmerge
#define xpack_meta_seq_tmerge
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tmerge::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tmerge{
    using namespace inc;

    template<class tlist0_t, class tlist1_t> struct tmerge;
    template<class ... args0_t, class ... args1_t> 
    struct tmerge<tlist<args0_t ...>, tlist<args1_t ...>>{
        using new_list = tlist<args0_t ..., args1_t ...>;
    };
}

#endif

xexport(mixc::meta_seq_tmerge::tmerge)