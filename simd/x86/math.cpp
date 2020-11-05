#define xuser mixc::simd_x86::inc
#include"define/base_type.hpp"
#include"intrin.h"
#include"math/const.hpp"
#include"math/sin.hpp"

#include"beginc"

auto sin_unsafe(f64 x[4]) {
    using namespace xuser;
    using namespace mixc::math_sin;

    constexpr auto step = 10;
    auto px         = _mm256_load_pd(x);

    // auto u2         = _mm256_mul_pd(px, px);
    // auto r0         = _mm256_setzero_pd();
    // auto k          = r0;
    // auto lut        = lut_f64x1;

    // for(uxx i = 0; i < step; i++){
    //     k           = _mm256_set1_pd(lut[i]);
    //     r0          = _mm256_fmadd_pd(k, px, r0);
    //     px          = _mm256_mul_pd(px, u2); 
    // }

    // self expand 
    // make the full pipe line
    auto lut        = lut_f64x1;

    auto u2         = _mm256_mul_pd(px, px);
    auto u4         = _mm256_mul_pd(u2, u2);
    auto u6         = _mm256_mul_pd(u4, u2);
    auto u8         = _mm256_mul_pd(u4, u4);
    auto u10        = _mm256_mul_pd(u4, u6);
    auto u12        = _mm256_mul_pd(u6, u6);
    auto u14        = _mm256_mul_pd(u6, u8);
    auto u16        = _mm256_mul_pd(u8, u8);
    auto u18        = _mm256_mul_pd(u8, u10);

    auto k0         = _mm256_set1_pd(lut[0]);
    auto k1         = _mm256_set1_pd(lut[1]);
    auto k2         = _mm256_set1_pd(lut[2]);
    auto k3         = _mm256_set1_pd(lut[3]);
    auto k4         = _mm256_set1_pd(lut[4]);
    auto k5         = _mm256_set1_pd(lut[5]);
    auto k6         = _mm256_set1_pd(lut[6]);
    auto k7         = _mm256_set1_pd(lut[7]);
    auto k8         = _mm256_set1_pd(lut[8]);
    auto k9         = _mm256_set1_pd(lut[9]);

    auto r0         = _mm256_mul_pd(k0, px );
    auto r1         = _mm256_mul_pd(k1, u2 );
    auto r2         = _mm256_mul_pd(k2, u4 );
    auto r3         = _mm256_mul_pd(k3, u6 );
    auto r4         = _mm256_mul_pd(k4, u8 );
    auto r5         = _mm256_mul_pd(k5, u10);
    auto r6         = _mm256_mul_pd(k6, u12);
    auto r7         = _mm256_mul_pd(k7, u14);
    auto r8         = _mm256_mul_pd(k8, u16);
    auto r9         = _mm256_mul_pd(k9, u18);

    r0              = _mm256_add_pd(r0, r1);
    r1              = _mm256_add_pd(r2, r3);
    r2              = _mm256_add_pd(r4, r5);
    r3              = _mm256_add_pd(r6, r7);
    r4              = _mm256_add_pd(r8, r3);

    r0              = _mm256_add_pd(r0, r1);
    r1              = _mm256_add_pd(r2, r3);
    r2              = _mm256_add_pd(r4, r0);

    r0              = _mm256_add_pd(r0, r1);
    return r0;
}

auto sin(f64 x[4]) {
    using namespace xuser;
    using namespace mixc::math_sin;

    auto px         = _mm256_load_pd(x);
    auto ppi        = _mm256_set1_pd(pi);               // parallel pi
    auto prp        = _mm256_set1_pd(1.0 / pi);         // parallel 1/pi
    auto php        = _mm256_set1_pd(0.5 * pi);         // parallel half pi
    auto pone       = _mm256_set1_epi64x(1);            // parallel 1
    auto adj        = _mm256_add_pd(px, php);           // adjust x
    auto pq         = _mm256_mul_pd(adj, prp);          // parallel quotient
    auto pqi32      = _mm256_cvtpd_epi32(pq);           // parallel quotient f64 -> i32 -> i64
    auto pqi64      = _mm256_cvtepu32_epi64(pqi32);
    auto pqf        = _mm256_cvtepi32_pd(pqi32);        // parallel quotient i32 -> f64
    auto prem       = _mm256_sub_pd(                    // parallel remainder
        adj, _mm256_mul_pd(pqf, ppi)
    );
    auto pswt       = _mm256_cmpeq_epi64(               // parallel switch
        _mm256_and_si256(pqi64, pone), pone
    );
    auto pswtn      = _mm256_xor_si256(                 // parallel switch negtive logic
        _mm256_set1_epi64x(-1), pswt
    );
    auto pr0        = _mm256_and_pd(                    // parallel branch 0
        _mm256_sub_pd(ppi, prem), _mm256_castsi256_pd(pswt)
    );
    auto pr1        = _mm256_or_pd(                     // parallel branch 1
        _mm256_and_pd(prem, _mm256_castsi256_pd(pswtn)), pr0
    );

    px              = _mm256_sub_pd(pr1, php);          // parallel adjust x

    return sin_unsafe((f64 *) & px);
}

#define xgen_cos(...)                           \
auto cos ## __VA_ARGS__(f64 x[4]) {             \
    using namespace xuser;                      \
    auto px         = _mm256_load_pd(x);        \
    auto php        = _mm256_set1_pd(0.5 * pi); \
    auto adj        = _mm256_sub_pd(px, php);   \
    return sin ## __VA_ARGS__((f64 *)& adj);    \
}

xgen_cos()
xgen_cos(_unsafe)
#undef  xgen_cos

#include"endc"
