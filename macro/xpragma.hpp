#pragma once
#include"configure/switch.hpp"

#if xis_msvc_native
    #define xpragma(...)        __pragma(__VA_ARGS__)
#else
    #define xpragma(...)        _Pragma(#__VA_ARGS__)
#endif