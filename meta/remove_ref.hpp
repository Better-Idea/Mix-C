#pragma once
#include"define/base_type.hpp"

namespace mixc{
    template<class a>
    struct remove_ref {
        using result = a;
    };

    template<class a>
    struct remove_ref<a &> {
        using result = a;
    };
}
