#ifndef xpack_meta_is_origin_stringx
#define xpack_meta_is_origin_stringx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_origin_stringx::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_origin_arrayx.hpp"
#include"meta/is_base_char.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_origin_stringx{
    template<class seq_t, class item_t>
    concept is_origin_stringx = 
        inc::is_base_char<inc::item_origin_of<seq_t>> and
        inc::is_origin_arrayx<seq_t, const item_t>;
}

#endif

xexport(mixc::meta_is_origin_stringx::is_origin_stringx)