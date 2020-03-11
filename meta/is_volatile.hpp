#pragma once

namespace mixc{
    namespace inner_is_volatile{
        template<class a> struct meta {
            static constexpr bool result = false;
        };

        template<class a> struct meta<volatile a> {
            static constexpr bool result = true;
        };
    }

    template<class a>
    constexpr bool is_volatile = mixc::inner_is_volatile::meta<a>::result;
}
