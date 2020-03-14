#pragma once

namespace mixc{
    template<class a>
    a atom_fetch_and(a * left, a right){
        return __atomic_fetch_and(left, right, 5);
    }
}
