#pragma once
#include"utility/memop/addressof.hpp"

namespace mixc{
    template<class target, class source> 
    inline target & cast(source const & value){
        return *(target *)addressof(value);
    }
}
