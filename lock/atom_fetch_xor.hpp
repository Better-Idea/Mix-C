#pragma once

namespace mixc{
    template<class a>
    a atom_fetch_xor(a * left, a right){
        return __atomic_fetch_xor(left, right, 5);
    }
}
