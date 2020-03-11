#pragma once

namespace mixc{
    template<class a>
    constexpr bool is_union = __is_union(a);
}
