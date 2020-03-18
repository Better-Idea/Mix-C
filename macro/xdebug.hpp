#pragma once
#ifdef xenable_debug_print
    #include<stdio.h>
    #include<typeinfo>
    #define xdebug(...)         printf(__VA_ARGS__)
#else
    #define xdebug(...)
#endif
