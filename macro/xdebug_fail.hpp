#pragma once
#ifdef xenable_xdebug_fail
    #include<stdio.h>

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
    #define xdebug_fail(...)
#endif
