#pragma once
#define xmixc                       "Mix-C"

// debug options
#define xuse_xdebug                 0
#define xuse_xdebug_fail            0
#define xuse_xdebug_short_path      1

// select memory allocator
#define xuse_libc_malloc            0

// select os-bits
#define xis_os64                    1
#define xis_os32                    0

// auto select os
#if     __WIN32__ or __WIN64__
    #define xis_windows             1
#elif   __linux__
    #define xis_linux               1
#endif

// auto select architecture
#if     __x86_64__
    #define xis_x86                 1
#elif   __arm__
    #define xis_arm                 1
#endif

#if     _MSVC_LANG
    #define xis_msvc                1
#endif

// check
#if xis_os64 + xis_os32 != 1
    static_assert(xis_os64 and (sizeof(voidp) == 8));
    static_assert(xis_os32 and (sizeof(voidp) == 4));
    #error "you can select only one between xis_os64 and xis_os32"
#endif
