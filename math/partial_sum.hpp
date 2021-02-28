#ifndef xpack_math_partial_sum
#define xpack_math_partial_sum
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_partial_sum::inc
#include"define/base_type.hpp"
#include"interface/ranger.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_partial_sum{
    template<class item_t>
    inline void partial_sum(
        inc::ranger<item_t> const & des, 
        inc::ranger<item_t> const & src){

        if (src.length() == 0){
            return;
        }

        auto tmp        = des[0] = src[0];

        for(uxx i = 1, length = src.length(); i < length; i++){
            auto last   = tmp;
            tmp         = des[i];
            des[i]      = last + src[i];
        }
    }

    template<class item_t>
    inline void partial_sum(inc::ranger<item_t> des_src){
        partial_sum<item_t>(des_src, des_src);
    }
}

#endif

xexport(mixc::math_partial_sum::partial_sum)