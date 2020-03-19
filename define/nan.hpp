#pragma once
#include"define/base_type.hpp"

namespace mixc{
    struct nan_t {
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
    };
    constexpr mixc::nan_t        nan {};
}
