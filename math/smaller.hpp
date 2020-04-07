#ifndef xpack_math_smaller
#define xpack_math_smaller
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_smaller
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_smaller{
        template<class type>
        inline type smaller(type left, type right){
            return left < right ? left : right;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::math_smaller::smaller;
}
