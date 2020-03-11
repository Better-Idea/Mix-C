#pragma once

namespace mixc{
    template<class a>
    constexpr bool is_enum = __is_enum(a);
}
