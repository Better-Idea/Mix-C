#ifndef xpack_meta_is_empty_class
#define xpack_meta_is_empty_class
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_empty_class::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_empty_class{
    template<class type_t>
    inline constexpr bool meta(){
        if constexpr (__is_class(type_t)){
            struct test : type_t{ char dummy; };
            return sizeof(test) == 1;
        }
        else{
            return false;
        }
    }

    template<class type_t>
    concept is_empty_class = meta<type_t>();
}

#endif

xexport(mixc::meta_is_empty_class::is_empty_class)