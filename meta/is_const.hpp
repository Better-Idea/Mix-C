#pragma once

namespace mixc{
    namespace inner_is_const{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<class a> struct meta<const a> {
            static constexpr bool result = true;
        };
    }

    template<class a>
    constexpr bool is_const = mixc::inner_is_const::meta<a>::result;
}
