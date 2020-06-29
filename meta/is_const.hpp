#ifndef xpack_meta_is_const
#define xpack_meta_is_const
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_const
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<class a> struct meta<const a> {
            static constexpr bool result = true;
        };

        template<class a>
        constexpr bool is_const = meta<a>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_const::is_const;
}
