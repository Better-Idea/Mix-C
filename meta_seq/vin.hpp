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

    template<auto val>
    inline constexpr bool meta(vlist<>){
        return false;
    }

    template<auto val, auto first, auto ... args>
    inline constexpr bool meta(vlist<first, args...>){
        if constexpr (first == val){
            return true;
        }
        else{
            return meta<val>(vlist<args...>());
        }
    }

    template<class vlist, auto item>
    constexpr bool vin = meta<item>(vlist());
}

#endif

xexport(mixc::meta_seq_vin::vin)