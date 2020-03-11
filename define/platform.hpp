#pragma once

constexpr bool          is_64bit = sizeof(void *) == 8;
constexpr bool          is_32bit = sizeof(void *) == 4;

enum{
    in_intel,
    in_arm,
    in_risc_v,
    in_mips,
    in_power_pc,
};

#ifndef xplatform
    #define xplatform   in_intel
#endif
