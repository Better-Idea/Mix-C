#pragma once
#include"macro/xlink.hpp"

#define xunused     [[maybe_unused]]
#define xunused_ret [[maybe_unused]] auto const & xlink2(__unused, __COUNTER__) = 