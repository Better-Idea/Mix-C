#ifndef xpack_interface_seqptr
#define xpack_interface_seqptr
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_seqptr::inc
#include"docker/private/adapter.array_access.hpp"
#include"interface/private/seqptr.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_seqptr::origin{
    using inc::can_seqptrlize;
    using inc::can_seqptrlizex;
    using inc::seqptr;
}

#endif

xexport_space(mixc::interface_seqptr::origin)