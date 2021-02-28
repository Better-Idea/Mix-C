#ifndef xpack_meta_is_ptr
#define xpack_meta_is_ptr
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_ptr::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_ptr{
    template<class type_t> struct meta {
        static constexpr bool result = false;
    };
    
    template<class type_t> struct meta<type_t *> {
        static constexpr bool result = true;
    };
    
    template<class type_t> struct meta<type_t const *> {
        static constexpr bool result = true;
    };

    template<class type_t> struct meta<type_t * const> {
        static constexpr bool result = true;
    };

    template<class type_t> struct meta<const type_t * const> {
        static constexpr bool result = true;
    };

    template<class type_t>
    concept is_ptr = meta<type_t>::result;
}

#endif

xexport(mixc::meta_is_ptr::is_ptr)