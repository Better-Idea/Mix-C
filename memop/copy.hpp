#pragma once
#include"utility/type/mirror.hpp"

namespace mixc{
    template<class a, class b>
    inline void copy(a * des, b const & src){
        using m = mixc::mirror<sizeof(a)>;
        ((m *)des)[0] = m(src);
    }

    template<class a>
    inline void copy_with_operator(a * target, a const * source, uxx count) {
        a * t = (a *)(target);
        a * s = (a *)(source);

        if (target > source) {
            while(count--){
                t[count] = s[count];
            }
        }
        else {
            for (uxx index = 0; index < count; index++) {
                t[index] = s[index];
            }
        }
    }

    template<class a>
    inline void copy(a * target, a const * source, uxx count) {
        using mp = mixc::mirror<sizeof(a)> *;
        mixc::copy_with_operator(mp(target), mp(source), count);
    }
}
