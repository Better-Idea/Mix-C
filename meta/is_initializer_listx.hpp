#ifndef xpack_meta_is_initializer_listx
#define xpack_meta_is_initializer_listx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_initializer_listx::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"interface/initializer_list.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_initializer_listx{
    template<class, class>
    struct meta{
        enum{ result = false };
    };

    template<class item_t>
    struct meta<std::initializer_list<item_t>, item_t>{
        enum{ result = true };
    };

    template<class may_list_t, class item_t>
    concept is_initializer_listx = meta<may_list_t, item_t>::result == true;
}

#endif

xexport(mixc::meta_is_initializer_listx::is_initializer_listx)