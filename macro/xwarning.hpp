#pragma once
#include"macro/xpragma.hpp"
#define xwarning_push()             xpragma(warning(push)) 
#define xwarning_pop()              xpragma(warning(pop)) 
#define xwarning_disable(...)       xpragma(warning(disable:__VA_ARGS__))
