#pragma once

using mixc_i08      = signed char;
using mixc_i16      = signed short;
using mixc_i32      = signed int;
using mixc_i64      = signed long long;
using mixc_u08      = unsigned char;
using mixc_u16      = unsigned short;
using mixc_u32      = unsigned int;
using mixc_u64      = unsigned long long;
using mixc_f32      = float;
using mixc_f64      = double;
using mixc_w16      = char16_t;
using mixc_asciis   = const char *;
using mixc_ci08     = const mixc_i08;
using mixc_ci16     = const mixc_i16;
using mixc_ci32     = const mixc_i32;
using mixc_ci64     = const mixc_i64;
using mixc_cu08     = const mixc_u08;
using mixc_cu16     = const mixc_u16;
using mixc_cu32     = const mixc_u32;
using mixc_cu64     = const mixc_u64;
using mixc_cf32     = const mixc_f32;
using mixc_cf64     = const mixc_f64;
using mixc_i08p     = mixc_i08  *;
using mixc_i16p     = mixc_i16  *;
using mixc_i32p     = mixc_i32  *;
using mixc_i64p     = mixc_i64  *;
using mixc_u08p     = mixc_u08  *;
using mixc_u16p     = mixc_u16  *;
using mixc_u32p     = mixc_u32  *;
using mixc_u64p     = mixc_u64  *;
using mixc_f32p     = mixc_f32  *;
using mixc_f64p     = mixc_f64  *;
using mixc_w16p     = mixc_w16  *;
using mixc_voidp    = void      *;


#define i08         mixc_i08
#define i16         mixc_i16
#define i32         mixc_i32
#define i64         mixc_i64
#define u08         mixc_u08
#define u16         mixc_u16
#define u32         mixc_u32
#define u64         mixc_u64
#define f32         mixc_f32
#define f64         mixc_f64
#define w16         mixc_w16
#define asciis      mixc_asciis
#define ci08        mixc_ci08
#define ci16        mixc_ci16
#define ci32        mixc_ci32
#define ci64        mixc_ci64
#define cu08        mixc_cu08
#define cu16        mixc_cu16
#define cu32        mixc_cu32
#define cu64        mixc_cu64
#define cf32        mixc_cf32
#define cf64        mixc_cf64
#define i08p        mixc_i08p
#define i16p        mixc_i16p
#define i32p        mixc_i32p
#define i64p        mixc_i64p
#define u08p        mixc_u08p
#define u16p        mixc_u16p
#define u32p        mixc_u32p
#define u64p        mixc_u64p
#define f32p        mixc_f32p
#define f64p        mixc_f64p
#define w16p        mixc_w16p
#define voidp       mixc_voidp

namespace mixc{
    template<int> struct env{};
    
    template<>
    struct env<4>{
        using ixx       = i32;
        using uxx       = u32;
    };

    template<>
    struct env<8>{
        using ixx       = i64;
        using uxx       = u64;
    };
    
    using ixx           = env<sizeof(void *)>::ixx;
    using uxx           = env<sizeof(void *)>::uxx;
}

using ixx               = mixc::ixx;
using uxx               = mixc::uxx;
using ixxp              = ixx   *;
using uxxp              = uxx   *;
constexpr uxx           not_exist = uxx(-1);
constexpr uxx           magic_number = 19961212;
