#ifndef xpack_meta_is_base_char
#define xpack_meta_is_base_char
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_base_char::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_base_char{
    template<class char_t>
    concept is_base_char = 
        inc::is_same<char_t, char> or 
        inc::is_same<char_t, char16_t> or 
        inc::is_same<char_t, char32_t>;

        // TODO:char8_t
}

#endif

xexport(mixc::meta_is_base_char::is_base_char)