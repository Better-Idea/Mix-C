#ifndef xpack_interface_ranger
#define xpack_interface_ranger
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_ranger::inc
#include"docker/private/adapter.array_access.hpp"
#include"interface/private/ranger.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_ranger::origin{
    using inc::can_rangerlize;
    using inc::can_rangerlizex;
    using inc::ranger;
}

#endif

xexport_space(mixc::interface_ranger::origin)