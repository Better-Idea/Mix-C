#ifndef xpack_math_numeration_t
#define xpack_math_numeration_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_numeration_t::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_numeration_t{
    enum class numeration_t : u08 {
        bin =  2,
        oct =  8,
        dec =  10,
        hex =  16,
    };
}

#endif

xexport(mixc::math_numeration_t::numeration_t)