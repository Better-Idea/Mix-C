#ifndef xpack_meta_seq_tlist
#define xpack_meta_seq_tlist
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tlist::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tlist{
    template<class ... args_t>
    struct tlist{
        static constexpr uxx length = sizeof...(args_t);
    };
}

#endif

xexport(mixc::meta_seq_tlist::tlist)