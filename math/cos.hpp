#ifndef xpack_math_cos
#define xpack_math_cos
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_cos::inc
#include"define/base_type.hpp"
#include"docker/array.hpp"
#include"macro/xexport.hpp"
#include"math/private/parallel_lut.hpp"
#include"math/const.hpp"
#include"math/sin.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_cos::origin{
    template<class type, class ... args>
    inline auto cos_unsafe(type x0, args ... xn) {
        return inc::sin_unsafe(x0 + inc::pi / 2, (xn + inc::pi / 2)...);
    }

    template<class type, class ... args>
    inline auto cos(type x0, args ... xn) {
        return inc::sin(x0 + inc::pi / 2, (xn + inc::pi / 2)...);
    }
}

#endif

xexport_space(mixc::math_cos::origin)