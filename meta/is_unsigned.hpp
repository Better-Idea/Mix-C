#ifndef xpack_meta_is_unsigned
#define xpack_meta_is_unsigned
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_unsigned
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<> struct meta<u08> {
            static constexpr bool result = true;
        };

        template<> struct meta<u16> {
            static constexpr bool result = true;
        };

        template<> struct meta<u32> {
            static constexpr bool result = true;
        };

        template<> struct meta<u64> {
            static constexpr bool result = true;
        };

        template<class a>
        constexpr bool is_unsigned = meta<a>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_unsigned::is_unsigned;
}
