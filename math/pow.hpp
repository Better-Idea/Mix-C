#ifndef xpack_math_pow
#define xpack_math_pow
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_pow
    #include"define/base_type.hpp"

    namespace xuser::origin{
        template<class type>
        inline type pow(type a, uxx x){
            type r       = 1;

            while(x){
                if (x & 1){
                    r *= a;
                }
                if (x >>= 1; x != 0){
                    a *= a;
                }
            }
            return r;
        }
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::math_pow::origin;
}
