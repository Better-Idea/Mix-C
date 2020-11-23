#ifndef xpack_interface_can_alloc
#define xpack_interface_can_alloc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_alloc::inc
#include"interface/can_callback.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_alloc::origin{
    template<class item_t>
    using ialloc = inc::icallback<item_t *(uxx)>;

    template<class functor_t, class item_t>
    concept can_alloc = inc::can_callback<functor_t, item_t *(uxx)>;
}

#endif

xexport_space(mixc::interface_can_alloc::origin)