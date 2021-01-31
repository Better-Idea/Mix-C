#pragma once
#include"configure.hpp"

#if xis_msvc_compatible
    #define xalign(...)     __declspec(align(__VA_ARGS__))
#else
    #define xalign(...)     __attribute__((aligned(__VA_ARGS__)))
#endif
