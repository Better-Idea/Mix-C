#pragma once

namespace mixc{
    template<class a>
    a atom_add(a * left, a right){
        return __atomic_add_fetch(left, right, 5);
    }
}
