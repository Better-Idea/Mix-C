#ifndef xpack_math_max
#define xpack_math_max
    namespace mixc::math_max{
        template<class type>
        inline type max(type left, type right){
            return left > right ? left : right;
        }
    }
#endif

namespace xuser::inc{
    using ::mixc::math_max::max;
}
