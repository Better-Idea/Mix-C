#pragma once
#include"macro/xnt.hpp"
#include"macro/private/xlist.hpp"

#define xlist_name(...)   __xlist__(xheader_1st_,xget_1st_,__VA_ARGS__)
