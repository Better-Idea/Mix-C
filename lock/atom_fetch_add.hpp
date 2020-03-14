#pragma once

namespace mixc{
    template<class a>
    a atom_fetch_add(a * left, a right){
        return __atomic_fetch_add(left, right, 5);
    }
}
