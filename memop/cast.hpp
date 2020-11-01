#ifndef xpack_memop_cast
#define xpack_memop_cast
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_cast::inc
#include"memop/addressof.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_cast{
    template<class target, class source> 
    inline target & cast(source const & value){
        return *(target *)inc::addressof(value);
    }
}

#endif

xexport(mixc::memop_cast::cast)