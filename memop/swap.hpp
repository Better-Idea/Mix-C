#pragma once

namespace mixc{
    template<class a>
    void swap(a * left, a * right){
        struct m{ char bytes[sizeof(a)]; };
        using mp = m *;
        m & l = mp(left)[0];
        m & r = mp(right)[0];
        m   t = l;
        l = r;
        r = t;
    }
}
