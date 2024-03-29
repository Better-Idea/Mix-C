#ifndef xpack_chrono_private_lut
#define xpack_chrono_private_lut
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_lookup_table::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_lookup_table::origin{
    constexpr u16 sum_leap  [] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
    constexpr u16 sum_normal[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
}
#endif

xexport_space(mixc::chrono_private_lookup_table::origin)
