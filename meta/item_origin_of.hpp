#ifndef xpack_meta_item_origin_of
#define xpack_meta_item_origin_of
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_item_origin_of::inc
#include"configure.hpp"
#include"macro/xexport.hpp"
#include"meta/is_indexable.hpp"
#include"meta/item_of.hpp"
#include"meta/remove_ref.hpp"
#include"meta/remove_const.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_item_origin_of{
    template<inc::is_indexable seq_t>
    using item_origin_of = 
        inc::remove_const<
            inc::remove_ref<
                inc::item_of<seq_t>
            >
        >;
}

#endif

xexport(mixc::meta_item_origin_of::item_origin_of)
