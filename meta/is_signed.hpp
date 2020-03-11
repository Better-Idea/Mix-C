#pragma once
#include"define/base_type.hpp"

namespace mixc{
    namespace inner_is_signed{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<> struct meta<i08> {
            static constexpr bool result = true;
        };

        template<> struct meta<i16> {
            static constexpr bool result = true;
        };

        template<> struct meta<i32> {
            static constexpr bool result = true;
        };

        template<> struct meta<i64> {
            static constexpr bool result = true;
        };
    }

    template<class a>
    constexpr bool is_signed = mixc::inner_is_signed::meta<a>::result;
}
