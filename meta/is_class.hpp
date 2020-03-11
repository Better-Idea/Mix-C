#pragma once

namespace mixc{
    template<class a>
    constexpr bool is_class = __is_class(a);
}
