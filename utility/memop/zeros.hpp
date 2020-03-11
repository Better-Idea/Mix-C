#pragma once
#include"define/base_type.hpp"

namespace mixc{
    inline void zeros(voidp target, uxx bytes) {
        while(bytes--){
            ((u08 *)(target))[bytes] = 0;
        }
    }

    template<class a>
    inline void zeros(a * ptr) {
        zeros(ptr, sizeof(a));
    }
}
