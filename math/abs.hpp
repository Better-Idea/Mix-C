#ifndef xpack_math_abs
#define xpack_math_abs
    namespace mixc::math_abs{
        template<class type>
        inline type abs(type const & value){
            return value < 0 ? -value : value;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::math_abs::abs;
}
