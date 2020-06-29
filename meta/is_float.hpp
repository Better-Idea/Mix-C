#ifndef xpack_meta_is_float
#define xpack_meta_is_float
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_float
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<> struct meta<f32> {
            static constexpr bool result = true;
        };

        template<> struct meta<f64> {
            static constexpr bool result = true;
        };

        template<class a>
        constexpr bool is_float = meta<a>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_float::is_float;
}
