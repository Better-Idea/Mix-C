#ifndef xpack_math_larger
#define xpack_math_larger
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_larger
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_larger{
        template<class type>
        inline type larger(type left, type right){
            return left > right ? left : right;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::math_larger::larger;
}
