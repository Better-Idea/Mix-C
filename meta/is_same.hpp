#pragma once

namespace mixc{
    namespace inner_is_same{
        template<class a, class b> struct meta {
            static constexpr bool result = false;
        };

        template<class a> struct meta<a, a> {
            static constexpr bool result = true;
        };
    }

    template<class a, class b>
    constexpr bool is_same = mixc::inner_is_same::meta<a, b>::result;
}
