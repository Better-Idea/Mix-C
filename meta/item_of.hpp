#ifndef xpack_meta_item_of
#define xpack_meta_item_of
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_item_of::inc
#include"interface/initializer_list.hpp"
#include"macro/xexport.hpp"
#include"meta/is_indexable.hpp"
#include"meta/remove_ref.hpp"
#include"meta/remove_const.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_item_of{
    template<class> struct meta;

    template<inc::is_indexable seq_t>
    struct meta<seq_t>{
        static decltype(auto) invoke(){
            seq_t * list = (seq_t *)magic_number;
            return list[0][0];
        }
        using result = decltype(invoke());
    };

    template<class item_t, uxx length_v>
    struct meta<item_t[length_v]>{
        using result = item_t;
    };

    template<class item_t>
    struct meta<inc::initializer_list<item_t>>{
        using result = item_t;
    };

    template<class seq_t>
    using item_of = typename meta<
        inc::remove_const<
            inc::remove_ref<seq_t>
        >
    >::result;
}

#endif

xexport(mixc::meta_item_of::item_of)
