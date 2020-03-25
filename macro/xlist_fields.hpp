#pragma once
#include"macro/private/xlist.hpp"
#include"macro/private/xprefix.hpp"

#define xlist_fields(...)   __xlist__(first_fields_,fields_,__VA_ARGS__)
