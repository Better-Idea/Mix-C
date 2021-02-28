#ifndef xpack_meta_is_unsigned
#define xpack_meta_is_unsigned
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_unsigned::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_unsigned{
    template<class type_t> struct meta {
        static constexpr bool result = false;
    };

    template<> struct meta<u08> {
        static constexpr bool result = true;
    };

    template<> struct meta<u16> {
        static constexpr bool result = true;
    };

    template<> struct meta<u32> {
        static constexpr bool result = true;
    };

    template<> struct meta<u64> {
        static constexpr bool result = true;
    };

    template<class type_t>
    concept is_unsigned = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_unsigned::is_unsigned)