#pragma once
#include"macro/private/xlist.hpp"
#include"macro/private/xprefix.hpp"

#define xlist_type(...)   __xlist__(first_2nd_,2nd_,__VA_ARGS__)
