#ifndef xpack_meta_seq_tin
#define xpack_meta_seq_tin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tin::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tin{
    using namespace inc;

    template<class item_t>
    inline constexpr bool invoke(tlist<>){
        return false;
    }

    template<class item_t, class first_t, class ... args_t>
    inline constexpr bool invoke(tlist<first_t, args_t...>){
        if constexpr (is_same<first_t, item_t>){
            return true;
        }
        else{
            return invoke<item_t>(tlist<args_t...>());
        }
    }

    template<class tlist, class item_t>
    constexpr bool tin = invoke<item_t>(tlist());
}

#endif

xexport(mixc::meta_seq_tin::tin)