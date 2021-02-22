#ifndef xpack_dumb_move
#define xpack_dumb_move
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_move::inc
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_move::origin{
    template<class type_t>
    inline type_t && move(type_t const & value){
        return (type_t &&)(type_t &)value;
    }
}

#endif

xexport_space(mixc::dumb_move::origin)
