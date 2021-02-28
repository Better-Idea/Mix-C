#ifndef xpack_meta_seq_tpop_front
#define xpack_meta_seq_tpop_front
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tpop_front::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tnull.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tpop_front{
    template<class list_t> struct tpop_front;
    template<class first_t, class ... args_t>
    struct tpop_front<inc::tlist<first_t, args_t...>>{
        using item_t    = first_t;
        using new_list  = inc::tlist<args_t...>;
    };

    template<>
    struct tpop_front<inc::tlist<>>{
        using item_t    = inc::tnull;
        using new_list  = inc::tlist<>;
    };
}

#endif

xexport(mixc::meta_seq_tpop_front::tpop_front)