#pragma once
#include"configure/switch.hpp"

#ifndef xis_msvc_native
    #define xlikely(...)      if (__builtin_expect(__VA_ARGS__, 1))
#else
    #define xlikely(...)      [[likely]] if (; __VA_ARGS__)
#endif
