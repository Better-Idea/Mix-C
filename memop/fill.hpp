#pragma once
#include"define/base_type.hpp"
#include"utility/type/mirror.hpp"

namespace mixc{
    template<class target, class source>
    inline void fill_with_operator(target * a, source const & value, uxx count) {
        for (uxx i = 0; i < count; i++) {
            a[i] = (source &)value;
        }
    }
    
    template<class target, class source>
    inline void fill(target * a, source const & value, uxx count) {
        using mp = mixc::mirror<sizeof(target)> *;
        fill_with_operator(mp(a), value, count);
    }
}
