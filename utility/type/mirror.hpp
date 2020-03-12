#pragma once
#include"define/base_type.hpp"
#include"memop/cast.hpp"

namespace mixc{
    template<uxx count>
    struct mirror{
        u08 bytes[count];
        static constexpr uxx length = count;

        mirror(): 
            bytes{0}{
        }

        template<class a>
        mirror(a const & value){
            this[0] = mixc::cast<mirror<length>>(value);
        }
    };
}
