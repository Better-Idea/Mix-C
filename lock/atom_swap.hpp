#pragma once

namespace mixc{
    template<class a>
    a atom_swap(a * left, a right){
        // function equals to 
        // r = left[0]
        // left[0] = right
        a r;
        __atomic_exchange(left, & right, & r, 5);
        return r;
    }
}
