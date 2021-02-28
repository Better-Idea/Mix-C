#ifndef xpack_meta_seq_vin
#define xpack_meta_seq_vin
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vin::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/vlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vin{
    using namespace inc;

    template<auto val_v>
    inline constexpr bool invoke(vlist<>){
        return false;
    }

    template<auto val_v, auto first, auto ... args_t>
    inline constexpr bool invoke(vlist<first, args_t...>){
        if constexpr (first == val_v){
            return true;
        }
        else{
            return invoke<val_v>(vlist<args_t...>{});
        }
    }

    template<class vlist, auto item_t>
    constexpr bool vin = invoke<item_t>(vlist{});
}

#endif

xexport(mixc::meta_seq_vin::vin)