#pragma once

namespace mixc{
    template<class a>
    a atom_or(a * left, a right){
        return __atomic_or_fetch(left, right, 5);
    }
}
