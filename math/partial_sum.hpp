#ifndef xpack_math_partial_sum
#define xpack_math_partial_sum
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_partial_sum
    #include"define/base_type.hpp"
    #include"interface/ranger.hpp"

    namespace xuser::origin{
        template<class item_t>
        inline void partial_sum(
            inc::ranger<item_t> des, 
            inc::ranger<item_t> src){

            if (src.length() == 0){
                return;
            }

            auto tmp = des[0] = src[0];

            for(uxx i = 1, length = src.length(); i < length; i++){
                auto last = tmp;
                tmp    = des[i];
                des[i] = last + src[i];
            }
        }

        template<class item_t>
        inline void partial_sum(inc::ranger<item_t> des_src){
            partial_sum<item_t>(des_src, des_src);
        }
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::math_partial_sum::origin;
}
