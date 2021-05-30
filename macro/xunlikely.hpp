#pragma once
#include"configure/platform.hpp"

#ifndef xis_msvc_native
    #define xunlikely(...)      if (__builtin_expect(__VA_ARGS__, 0))
#else
    #define xunlikely(...)      [[unlikely]] if (; __VA_ARGS__)
#endif
