#pragma once
#include"macro/private/xlist.hpp"
#include"macro/private/xprefix.hpp"

#define xlist_args(...)   __xlist__(first_arg_,arg_,__VA_ARGS__)
