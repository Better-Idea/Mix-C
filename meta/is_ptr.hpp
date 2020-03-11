#pragma once

namespace mixc{
    namespace inner_is_ptr{
        template<class a> struct meta {
            static constexpr bool result = false;
        };
        
        template<class a> struct meta<a *> {
            static constexpr bool result = true;
        };
    }

    template<class a>
    constexpr bool is_ptr = mixc::inner_is_ptr::meta<a>::result;
}
