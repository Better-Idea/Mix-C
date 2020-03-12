#pragma once

namespace mixc{
    template<class a> 
    inline a * addressof(a const & value){
        struct inner{
            inner(a const & value) : value(value){ }
            a const & value;
        } got(value);
        return (*(a **)& got);
    }
}
