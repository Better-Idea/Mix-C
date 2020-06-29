#ifndef xpack_meta_is_ptr
#define xpack_meta_is_ptr
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_ptr
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a> struct meta {
            static constexpr bool result = false;
        };
        
        template<class a> struct meta<a *> {
            static constexpr bool result = true;
        };
        
        template<class a> struct meta<a const *> {
            static constexpr bool result = true;
        };

        template<class a> struct meta<a * const> {
            static constexpr bool result = true;
        };

        template<class a> struct meta<const a * const> {
            static constexpr bool result = true;
        };

        template<class a>
        constexpr bool is_ptr = meta<a>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_ptr::is_ptr;
}
