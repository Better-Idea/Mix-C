#ifndef xpack_meta_is_integer
#define xpack_meta_is_integer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_integer::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_signed.hpp"
#include"meta/is_unsigned.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_integer{
    template<class type_t>
    struct meta {
        static constexpr bool result = 
            inc::is_signed<type_t> or inc::is_unsigned<type_t>;
    };

    template<class type_t>
    concept is_integer = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_integer::is_integer)