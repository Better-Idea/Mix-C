#pragma once

namespace mixc{
    template<class a>
    a atom_and(a * left, a right){
        return __atomic_and_fetch(left, right, 5);
    }
}
