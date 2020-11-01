#ifndef xpack_dumb_disable_copy
#define xpack_dumb_disable_copy
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_disable_copy::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_disable_copy {
    xstruct(
        xname(disable_copy)
    )
        disable_copy() = default;
        disable_copy(disable_copy const &) = delete;
        void operator=(disable_copy const &) = delete;
    $
}

#endif

xexport(mixc::dumb_disable_copy::disable_copy)