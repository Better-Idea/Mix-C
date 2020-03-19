#pragma once
#include"macro/xnt.hpp"
#include"macro/private/xlist.hpp"

#define xlist_args(...)   __xlist__(xheader_arg_,xget_arg_,__VA_ARGS__)
