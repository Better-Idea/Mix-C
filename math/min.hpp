#ifndef xpack_math_min
#define xpack_math_min
    namespace mixc::math_min{
        template<class type>
        inline type min(type left, type right){
            return left < right ? left : right;
        }
    }
#endif

namespace xuser::inc{
    using ::mixc::math_min::min;
}
