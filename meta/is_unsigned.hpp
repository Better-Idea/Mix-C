#pragma once
#include"define/base_type.hpp"

namespace mixc{
    namespace inner_is_unsigned{
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

    }

    template<class a>
    constexpr bool is_unsigned = mixc::inner_is_unsigned::meta<a>::result;
}
