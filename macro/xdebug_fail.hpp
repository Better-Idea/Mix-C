#ifndef xpack_macro_xdebug_fail
#define xpack_macro_xdebug_fail
    #pragma warning(disable:4390)
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xdebug_fail
    #include"configure.hpp"
    #include"macro/private/log.hpp"

    #if xuse_xdebug_fail
        #define xdebug_fail(...)                                                    \
            if (__builtin_expect(([](const char * name, bool error){                \
                if (not error){                                                     \
                    return false;                                                   \
                }                                                                   \
                else{                                                               \
                    using namespace ::mixc::macro_private_log::origin;              \
                    log(for_fail, __FILE__, __LINE__, name, # __VA_ARGS__);         \
                    return error;                                                   \
                }                                                                   \
            })(__func__, __VA_ARGS__), 0))
    #else
        #define xdebug_fail(...)  if (false)
    #endif

    #pragma pop_macro("xuser")
#endif
