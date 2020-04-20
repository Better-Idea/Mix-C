/*
可选硬件平台
- xin_x86
- xin_arm
- xin_risc_v
- xin_mips
*/
#pragma once

constexpr bool is_64bit          = sizeof(void *) == 8;
constexpr bool is_32bit          = sizeof(void *) == 4;

#ifdef __linux__
    #define xis_linux       1
#else
    #define xis_linux       0
#endif

#ifndef xis_x86
    #define xis_x86         0
#else
    constexpr bool stack_down_group = true;
#endif

#ifndef xis_arm
    #define xis_arm         0
#endif

#ifndef xis_risc_v
    #define xis_risc_v      0
#endif

#ifndef xis_mips
    #define xis_mips        0
#endif

#if not (xos64 or xos32)
    #error "you need define one os mode like \"#define xos64    1\""
#endif

#if not (xis_x86 || xis_arm || xis_risc_v || xis_mips)
    #error "you need select one platform"
#endif
