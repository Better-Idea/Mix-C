#ifndef xpack_meta_is_initializer_list
#define xpack_meta_is_initializer_list
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_initializer_list::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"interface/initializer_list.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_initializer_list{
    template<class type_t>
    struct meta{
        enum{ result = false };
    };

    template<class item_t>
    struct meta<std::initializer_list<item_t>>{
        enum{ result = true };
    };

    template<class type_t>
    concept is_initializer_list = meta<type_t>::result == true;
}

#endif

xexport(mixc::meta_is_initializer_list::is_initializer_list)