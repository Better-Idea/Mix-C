#ifndef xpack_meta_is_ref
#define xpack_meta_is_ref
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_ref
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<class a> struct meta<a &> {
            static constexpr bool result = true;
        };

        template<class a>
        constexpr bool is_ref = meta<a>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_ref::is_ref;
}
