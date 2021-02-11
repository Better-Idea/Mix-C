#pragma once
#include"macro/xpragma.hpp"
#define xwarning_disable(...)       xpragma(warning(push)) xpragma(warning(disable:__VA_ARGS__))
#define xwarning_enable()           xpragma(warning(pop))