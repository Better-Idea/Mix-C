#ifndef xpack_define_nan
#define xpack_define_nan
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_nan
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_nan{
    struct nan_t{
        constexpr nan_t(){}

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

        operator f32 () const{
            auto v = u32(-1) >> 1;
            return *(f32 *) & v;
        }

        operator f64 () const {
            auto v = u64(-1) >> 1;
            return *(f64 *) & v;
        }
    };

    constexpr nan_t nan {};
}
#endif

namespace xuser::inc{
    using ::mixc::define_nan::nan;
}
