#pragma once
#include"configure.hpp"

#ifndef xis_msvc
    #define xlikely(...)      if (__builtin_expect(__VA_ARGS__, 1))
#else
    #define xlikely(...)      [[likely]] if (; __VA_ARGS__)
#endif
