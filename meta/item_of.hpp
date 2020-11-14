#ifndef xpack_meta_item_of
#define xpack_meta_item_of
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_item_of::inc
#include"interface/initializer_list.hpp"
#include"meta/is_indexable.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_item_of{
    template<inc::is_indexable seq_t>
    struct meta{
        static decltype(auto) invoke(){
            seq_t * list = (seq_t *)magic_number;
            return list[0][0];
        }
        using result = decltype(invoke());
    };

    template<class item_t, uxx len>
    struct meta<item_t[len]>{
        using result = item_t;
    };

    template<class item_t>
    struct meta<inc::initializer_list<item_t>>{
        using result = item_t;
    };

    template<class seq_t>
    using item_of = typename meta<seq_t>::result;
}

#endif

xexport(mixc::meta_item_of::item_of)
