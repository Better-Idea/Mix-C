#ifndef xpack_meta_is_number
#define xpack_meta_is_number
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_number::inc
#include"define/base_type.hpp"
#include"is_integer.hpp"
#include"is_float.hpp"
#include"is_enum.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_number{
    template<class type_t> struct meta {
        static constexpr bool result = 
            inc::is_integer<type_t> or inc::is_float<type_t> or inc::is_enum<type_t>;
    };

    template<class type_t>
    concept is_number = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_number::is_number)