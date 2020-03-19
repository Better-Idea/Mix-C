#pragma once
#include"macro/xnt.hpp"
#include"macro/private/xlist.hpp"

#define xlist_fields(...)   __xlist__(xheader_fields_,xget_fields_,__VA_ARGS__)
