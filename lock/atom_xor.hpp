#pragma once

namespace mixc{
    template<class a>
    a atom_xor(a * left, a right){
        return __atomic_xor_fetch(left, right, 5);
    }
}
