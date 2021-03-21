#ifndef xpack_macro_xdebug_fail
#define xpack_macro_xdebug_fail
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xdebug_fail::inc
#include"macro/xunlikely.hpp"
#include"macro/private/log.hpp"
#include"macro/xwarning.hpp"
#pragma pop_macro("xuser")

#if xuse_xdebug_fail
    #define xdebug_fail(...)                                                    \
        xunlikely(([](const char * func, bool error){                           \
            if (not error){                                                     \
                return false;                                                   \
            }                                                                   \
            else{                                                               \
                using namespace ::mixc::macro_private_log::origin;              \
                log(for_fail, __FILE__, __LINE__, func, # __VA_ARGS__, fail);   \
                return error;                                                   \
            }                                                                   \
        })(__func__, __VA_ARGS__))
    xwarning_disable(4390)
#else
    #define xdebug_fail(...)  if (false)
#endif
#endif
