#ifndef xpack_math_max
#define xpack_math_max
#ifdef max
#undef max
#endif

#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_max::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_max::origin{
    template<class type_t>
    inline type_t const & max(type_t const & left, type_t const & right){
        return left > right ? left : right;
    }
}

#endif

xexport_space(mixc::math_max::origin)