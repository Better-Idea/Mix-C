#ifndef xpack_meta_is_origin_string
#define xpack_meta_is_origin_string
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_origin_string::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_origin_array.hpp"
#include"meta/is_base_char.hpp"
#include"meta/is_const.hpp"
#include"meta/item_of.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_origin_string{
    template<class seq_t>
    concept is_origin_string = 
        inc::is_base_char<inc::item_origin_of<seq_t>> and
        inc::is_origin_array<seq_t> and
        inc::is_const<inc::item_of<seq_t>>;
}

#endif

xexport(mixc::meta_is_origin_string::is_origin_string)