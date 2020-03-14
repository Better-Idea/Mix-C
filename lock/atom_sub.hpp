#pragma once

namespace mixc{
    template<class a>
    a atom_sub(a * left, a right){
        return __atomic_sub_fetch(left, right, 5);
    }
}
