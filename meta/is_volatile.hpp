#pragma once
#ifndef xpack_meta_is_volatile
#define xpack_meta_is_volatile
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_volatile
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_volatile{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<class a> struct meta<volatile a> {
            static constexpr bool result = true;
        };

        template<class a>
        constexpr bool is_volatile = meta<a>::result;
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_is_volatile::is_volatile;
}
