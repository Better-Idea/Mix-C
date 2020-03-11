#pragma once
#include"meta/is_unsigned.hpp"
#include"meta/is_signed.hpp"

namespace mixc{
    namespace inner_is_integer{
        template<class a>
        struct meta {
            static constexpr bool result = mixc::is_signed<a> || mixc::is_unsigned<a>;
        };
    }

    template<class a>
    constexpr bool is_integer = mixc::inner_is_integer::meta<a>::result;
}
