#pragma once
#include"define/base_type.hpp"

namespace mixc{
    namespace inner_is_float{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<> struct meta<f32> {
            static constexpr bool result = true;
        };

        template<> struct meta<f64> {
            static constexpr bool result = true;
        };
    }

    template<class a>
    constexpr bool is_float = mixc::inner_is_float::meta<a>::result;
}
