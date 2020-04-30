#pragma once
#define xmixc                       "Mix-C"

#define xuse_xdebug                 1
#define xuse_xdebug_fail            1
#define xuse_xdebug_short_path      1

// select os
#define xis_windows                 1
#define xis_linux                   0
#define xis_osx                     0

#if xis_windows + xis_linux + xis_osx != 1
    #error "you can select only one operation system"
#endif

// select architecture
#define xis_x86                     1
#define xis_arm                     0
#define xis_risc_v                  0
#define xis_mips                    0

#if xis_x86 + xis_arm + xis_risc_v + xis_mips != 1
    #error "you can select only one cpu platform"
#endif

// select os-bits
#define xis_os64                    1
#define xis_os32                    0

#if xis_os64 + xis_os32 != 1
    static_assert(xis_os64 and (sizeof(voidp) == 8));
    static_assert(xis_os32 and (sizeof(voidp) == 4));
    #error "you can select only one between xis_os64 and xis_os32"
#endif