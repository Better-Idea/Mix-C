#ifndef xpack_math_abs
#define xpack_math_abs
#include"macro/xexport.hpp"

namespace mixc::math_abs::origin{
    template<class type>
    inline type abs(type const & value){
        return value < 0 ? -value : value;
    }
}

#endif

xexport_space(mixc::math_abs::origin)