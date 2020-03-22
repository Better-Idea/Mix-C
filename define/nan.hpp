#ifndef xpack_define_nan
#define xpack_define_nan
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::define_nan
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::define_nan{
        xgc(nan)
            xgc_fields();

            friend inline auto operator == (f32 value, nan_t) {
                return !(value == value);
            }

            friend inline auto operator == (nan_t, f32 value) {
                return !(value == value);
            }

            friend inline auto operator == (f64 value, nan_t) {
                return !(value == value);
            }

            friend inline auto operator == (nan_t, f64 value) {
                return !(value == value);
            }

            friend inline auto operator != (f32 value, nan_t) {
                return (value == value);
            }

            friend inline auto operator != (nan_t, f32 value) {
                return (value == value);
            }

            friend inline auto operator != (f64 value, nan_t) {
                return (value == value);
            }

            friend inline auto operator != (nan_t, f64 value) {
                return (value == value);
            }
        xgc_end();

        constexpr nan_t nan {};
    }

#endif

namespace xuser::inc{
    using namespace mixc::define_nan;
}
