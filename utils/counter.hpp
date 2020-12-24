#ifndef xpack_utils_counter
#define xpack_utils_counter
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_counter::inc
#include"define/base_type.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_counter::origin{
    xstruct(
        xname(counter),
        xprif(value, uxx)
    )
        constexpr counter(uxx value = 0) : 
            value(value){
        }

        operator uxx () const {
            return value++;
        }
    $
}

#endif

xexport_space(mixc::utils_counter::origin)
