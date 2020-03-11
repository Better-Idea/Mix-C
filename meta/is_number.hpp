#pragma once
#include"is_integer.hpp"
#include"is_float.hpp"
#include"is_enum.hpp"

namespace mixc{
    namespace inner_is_number{
        template<class a> struct meta {
            static constexpr bool result = mixc::is_integer<a> || mixc::is_float<a> || mixc::is_enum<a>;
        };
    }

    template<class a>
    constexpr bool is_number = mixc::inner_is_number::meta<a>::result;
}
