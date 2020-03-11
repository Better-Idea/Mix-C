#pragma once
#include"define/base_type.hpp"

namespace mixc {
    template<class a>
    struct remove_const {
        using result = a;
    };

    template<class a>
    struct remove_const<const a> {
        using result = a;
    };
}
