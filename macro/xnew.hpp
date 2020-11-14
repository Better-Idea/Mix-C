#pragma once
#include"macro/xunused.hpp"
#include<new>

#define xnew(...)           xunused_ret new(__VA_ARGS__)