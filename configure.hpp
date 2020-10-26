#pragma once
#define xmixc                       "Mix-C"

/* mixc::xdebug ====================================================
 * 
 * =================================================================*/
#define xuse_xdebug                 0
#define xuse_xdebug_fail            0
#define xuse_xdebug_short_path      1

/* mixc::math =======================================================
 * 
 * =================================================================*/
#define xuse_math_tan_lut8x16       1
#define xuse_math_tan_lut4x256      0

#if xuse_math_tan_lut8x16 + xuse_math_tan_lut4x256 != 1
    #error "you can just select one lookup table policy for math::tan"
#endif

#define xuse_math_atan_lut7x16      1
#define xuse_math_atan_lut4x256     0
#if xuse_math_atan_lut7x16 + xuse_math_atan_lut4x256 != 1
    #error "you can just select one lookup table policy for math::atan"
#endif

#define xuse_math_ln_lut6x16        1
#define xuse_math_ln_lut3x256       0

#if xuse_math_ln_lut6x16 + xuse_math_ln_lut3x256 != 1
    #error "you can just select one lookup table policy for math::ln"
#endif

#define xuse_math_expe_lut6x16      1
#define xuse_math_expe_lut3x256     0

#if xuse_math_expe_lut6x16 + xuse_math_expe_lut3x256 != 1
    #error "you can just select one lookup table policy for math::expe"
#endif

#define xuse_math_sqrt_lut6x16      1
#define xuse_math_sqrt_lut3x256     0

#if xuse_math_sqrt_lut6x16 + xuse_math_sqrt_lut3x256 != 1
    #error "you can just select one lookup table policy for math::sqrt"
#endif

/* mixc::memory =====================================================
 * 
 * =================================================================*/

// select memory allocator
#define xuse_libc_malloc            1

/* mixc::auto =======================================================
 * 
 * =================================================================*/

// auto select os-bits
#if __i386 || __i386__ || __i686  || __i686__
    #define xis_os32                1
#elif __x86_64 || __x86_64__ || _M_X64
    #define xis_os64                1
#else
    #define xis_os32                1
#endif

// auto select os
#if _WIN32 || _WIN64 || __WIN32__ || __WIN64__
    #define xis_windows             1
#elif   __linux__ || __linux
    #define xis_linux               1
#endif

// auto select architecture
#if __i386 || _X86_ || __x86_64 || __x86_64__ || _M_IX86 || _M_X64
    #define xis_x86                 1
#elif   __arm__
    #define xis_arm                 1
#endif

#if _MSVC_LANG
    #define xis_msvc                1
#endif

#if __clang__
    #define xis_clang               1
#endif

// check
#if xis_os64 + xis_os32 != 1
    static_assert(xis_os64 and (sizeof(voidp) == 8));
    static_assert(xis_os32 and (sizeof(voidp) == 4));
    #error "you can select only one between xis_os64 and xis_os32"
#endif
