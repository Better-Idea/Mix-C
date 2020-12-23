#ifndef xpack_dumb_move
#define xpack_dumb_move
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_move::inc
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_move::origin{
    template<class type>
    inline type && move(type const & value){
        return (type &&)(type &)value;
    }
}

#endif

xexport_space(mixc::dumb_move::origin)
