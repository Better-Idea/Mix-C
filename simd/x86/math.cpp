#define xuser mixc::simd_x86::inc
#include"define/base_type.hpp"
#include"intrin.h"
#include"math/const.hpp"
#include"math/sin.hpp"

#include"beginc"

auto sin(f64 x0, f64 x1, f64 x2, f64 x3, bool safe) {
    using namespace xuser;
    using namespace mixc::math_sin;

    constexpr auto step = 10;
    auto px     = _mm256_setr_pd(x0, x1, x2, x3);

    if (safe){
        auto ppi    = _mm256_set1_pd(pi);               // parallel pi
        auto prp    = _mm256_set1_pd(1.0 / pi);         // parallel 1/pi
        auto php    = _mm256_set1_pd(0.5 * pi);         // parallel half pi
        auto pone   = _mm256_set1_epi64x(1);            // parallel 1
        auto adj    = _mm256_add_pd(px, php);           // adjust x
        auto pq     = _mm256_mul_pd(adj, prp);          // parallel quotient
        auto pqi32  = _mm256_cvtpd_epi32(pq);           // parallel quotient f64 -> i32 -> i64
        auto pqi64  = _mm256_cvtepu32_epi64(pqi32);
        auto pqf    = _mm256_cvtepi32_pd(pqi32);        // parallel quotient i32 -> f64
        auto prem   = _mm256_sub_pd(                    // parallel remainder
            adj, _mm256_mul_pd(pqf, ppi)
        );
        auto pswt   = _mm256_cmpeq_epi64(               // parallel switch
            _mm256_and_si256(pqi64, pone), pone
        );
        auto pswtn  = _mm256_xor_si256(                 // parallel switch negtive logic
            _mm256_set1_epi64x(-1), pswt
        );
        auto pr0    = _mm256_and_pd(                    // parallel branch 0
            _mm256_sub_pd(ppi, prem), _mm256_castsi256_pd(pswt)
        );
        auto pr1    = _mm256_or_pd(                     // parallel branch 1
            _mm256_and_pd(prem, _mm256_castsi256_pd(pswtn)), pr0
        );

        px          = _mm256_sub_pd(pr1, php);          // parallel adjust x
    }

    auto xx         = _mm256_mul_pd(px, px);
    auto rx         = _mm256_setzero_pd();
    auto k          = rx;
    auto lut        = lut_f64x4;

    for(uxx i = 0; i < step; i++, lut += 4){
        k           = _mm256_load_pd(lut);
        rx          = _mm256_fmadd_pd(k, px, rx);
        px          = _mm256_mul_pd(px, xx);
    }

    return rx;
}

auto cos(f64 x0, f64 x1, f64 x2, f64 x3, bool safe) {
    using namespace xuser;
    return sin(x0 - pi / 2, x1 - pi / 2, x2 - pi / 2, x3 - pi / 2);
}

#include"endc"
