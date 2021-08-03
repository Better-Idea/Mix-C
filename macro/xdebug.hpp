#ifndef xpack_macro_xdebug
#define xpack_macro_xdebug
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xdebug::inc
#include"configure/switch.hpp"
#include"macro/private/log.hpp"
#include"macro/xlink.hpp"
#include"macro/xwarning.hpp"
#pragma pop_macro("xuser")

#if xuse_xdebug
    #define xdebug(token,...)                               \
    if (token > 0 and ([&](asciis func){                    \
        using namespace ::mixc::macro_private_log::origin;  \
        log(                                                \
            for_debug, __FILE__, __LINE__,                  \
            func, #__VA_ARGS__ ",", normal, __VA_ARGS__);   \
        return true;                                        \
    })(__func__))

    xwarning_disable(4390)
#else
    #define xdebug(...)
#endif
#endif
