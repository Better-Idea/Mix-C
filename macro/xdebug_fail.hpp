#ifndef xpack_macro_xdebug_fail
#define xpack_macro_xdebug_fail
    #pragma warning(disable:4390)
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xdebug_fail
        #include"configure.hpp"
        #include<stdio.h>
    #pragma pop_macro("xuser")

    #if xuse_xdebug_fail
        #define xdebug_fail(...)                                                    \
            if (__builtin_expect(([](const char * name, bool error){                \
                if (not error){                                                     \
                    return false;                                                   \
                }                                                                   \
                else{                                                               \
                    printf(" FAIL | " __FILE__ ":%d\n", __LINE__);                  \
                    printf("      | func:%s expr:" # __VA_ARGS__ "\n", name);       \
                    return error;                                                   \
                }                                                                   \
            })(__func__, __VA_ARGS__), 0))
    #else
        #define xdebug_fail(...)  if (false)
    #endif
#endif
