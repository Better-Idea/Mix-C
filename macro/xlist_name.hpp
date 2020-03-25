#pragma once
#include"macro/private/xlist.hpp"
#include"macro/private/xprefix.hpp"

#define xlist_name(...)   __xlist__(first_1st_,1st_,__VA_ARGS__)
