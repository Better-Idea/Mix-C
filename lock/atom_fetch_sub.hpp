#pragma once

namespace mixc{
    template<class a>
    a atom_fetch_sub(a * left, a right){
        return __atomic_fetch_sub(left, right, 5);
    }
}
