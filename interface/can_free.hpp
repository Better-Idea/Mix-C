#ifndef xpack_interface_can_free
#define xpack_interface_can_free
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_free::inc
#include"interface/can_callback.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_free::origin{
    template<class item_t>
    using ifree = inc::icallback<void(item_t *, uxx)>;

    template<class functor_t, class item_t>
    concept can_free = inc::can_callback<functor_t, void(item_t *, uxx)>;
}

#endif

xexport_space(mixc::interface_can_free::origin)