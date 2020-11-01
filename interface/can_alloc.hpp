#ifndef xpack_interface_can_alloc
#define xpack_interface_can_alloc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_alloc::inc
#include"interface/can_callback.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_alloc{
    template<class item_t>
    using can_alloc = inc::can_callback<item_t *(uxx)>;
}

#endif

xexport(mixc::interface_can_alloc::can_alloc)