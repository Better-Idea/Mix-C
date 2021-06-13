#ifndef xpack_meta_char_type
#define xpack_meta_char_type
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_char_type::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_char_type{
    template<uxx>
    struct meta{};
    template<> struct meta<1> { using result = char; };
    template<> struct meta<2> { using result = char16_t; };
    template<> struct meta<4> { using result = char32_t; };

    template<class type_t>
    using char_type = typename meta<sizeof(type_t)>::result;
}

#endif

xexport(mixc::meta_char_type::char_type)