#ifndef xpack_meta_seq_tfilter
#define xpack_meta_seq_tfilter
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tfilter::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tfilter{
    using namespace inc;

    template<template <class> class selector_t, class ... result_args_t>
    inline auto invoke(tlist<>, tlist<result_args_t...> r){
        return r;
    }

    template<
        template <class> class  selector_t, 
        class                   first_t, 
        class ...               args_t, 
        class ...               result_args_t
    >
    inline auto invoke(tlist<first_t, args_t...>, tlist<result_args_t...>){
        using item_t = typename selector_t<first_t>::item_t;
        return invoke<selector_t>(
            tlist<args_t...>{}, 
            tlist<result_args_t..., item_t>{}
        );
    }

    template<class tlist_t, template <class> class selector_t>
    struct tfilter{
        using new_list = decltype(
            invoke<selector_t>(tlist_t{}, tlist<>{})
        );
    };
}

#endif

xexport(mixc::meta_seq_tfilter::tfilter)