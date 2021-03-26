#ifndef xpack_configure_platform
#define xpack_configure_platform

// auto select os-bits
#if   __SIZE_WIDTH__ == 64 or _WIN64
    #define xis_os64                1
#elif __SIZE_WIDTH__ == 32 or _WIN32
    #define xis_os32                1
#else
    #error "unknown os bits"
#endif

// auto select os
#if _WIN32 || _WIN64 || __WIN32__ || __WIN64__ || __CYGWIN__
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

#if __clang__
    #define xis_clang               1
#endif

#if _MSVC_LANG
    #define xis_msvc_compatible     1

    #ifndef xis_clang
        #define xis_msvc_native     1
    #endif
#endif

#if xis_windows
    #define xlocal_endl             "\r\n"
    #define xlocal_endl_length      2
#elif xis_linux
    #define xlocal_endl             "\n"
    #define xlocal_endl_length      1
#endif

#endif
