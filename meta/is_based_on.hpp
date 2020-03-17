#pragma once
#include"meta/is_same.hpp"

namespace mixc{
    template<class father, class child>
    constexpr bool is_based_on = __is_base_of(father, child);
}
