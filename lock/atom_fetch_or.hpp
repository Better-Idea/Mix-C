#pragma once

namespace mixc{
    template<class a>
    a atom_fetch_or(a * left, a right){
        return __atomic_fetch_or(left, right, 5);
    }
}
