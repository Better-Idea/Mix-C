#ifndef xpack_meta_is_indexable
#define xpack_meta_is_indexable
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_indexable::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_indexable{
    template<class seq_t>
    concept is_indexable = requires(seq_t items){
        items[0];
    };
}

#endif

xexport(mixc::meta_is_indexable::is_indexable)
