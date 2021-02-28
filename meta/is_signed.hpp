#ifndef xpack_meta_is_signed
#define xpack_meta_is_signed
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_signed::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_signed{
    template<class type_t> struct meta {
        static constexpr bool result = false;
    };

    template<> struct meta<i08> {
        static constexpr bool result = true;
    };

    template<> struct meta<i16> {
        static constexpr bool result = true;
    };

    template<> struct meta<i32> {
        static constexpr bool result = true;
    };

    template<> struct meta<i64> {
        static constexpr bool result = true;
    };

    template<class type_t>
    concept is_signed = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_signed::is_signed)