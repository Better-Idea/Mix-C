#define xuser mixc::simd_x86::inc
#include"define/base_type.hpp"
#include"intrin.h"
#include"math/const.hpp"
#include"math/sin.hpp"
#include"memop/swap.hpp"

template<class item_t>
requires(
    sizeof(item_t) == 8 or
    sizeof(item_t) == 4 or
    sizeof(item_t) == 2
)
inline void swap_bytes(item_t * buffer,item_t const * source, uxx length){
    using namespace xuser;

    constexpr
    auto step           = (32/*sizeof(ymm)*/ / sizeof(item_t));
    auto i              = (uxx)0;
    auto pidx           = 
        sizeof(item_t) == 2 ? _mm256_setr_epi8(
            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 29, 28, 31, 30
        ) :
        sizeof(item_t) == 4 ? _mm256_setr_epi8(
            3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28
        )                   : _mm256_setr_epi8(
            7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8, 23, 22, 21, 20, 19, 18, 17, 16, 31, 30, 29, 28, 27, 26, 25, 24
        );

    for(; i < length / step; i++){
        auto pbytes     = _mm256_castpd_si256(_mm256_loadu_pd(f64p(source + i * step)));
        auto pword      = _mm256_shuffle_epi8(pbytes, pidx);
        _mm256_storeu_pd(f64p(buffer + i * step), _mm256_castsi256_pd(pword));
    }

    for(i *= step; i < length; i++){
        buffer[i]       = source[i];

        auto begin      = u08p(buffer + i);
        auto end        = u08p(buffer + i + 1) - 1;

        for(uxx i = 0; i < sizeof(item_t) / 2; i++){
            swap(xref begin[i], xref end[uxx(0) - i]);
        }
    }
}


#include"beginc"

void topk(i32 k[8], i32 const * ary, uxx length){
    using namespace xuser;
    auto ptopa          = _mm256_set1_epi32(min_value_of<i32>);
    auto ptopb          = _mm256_set1_epi32(min_value_of<i32>);
    auto ptopc          = _mm256_set1_epi32(min_value_of<i32>);
    auto ptopd          = _mm256_set1_epi32(min_value_of<i32>);
    auto pidx           = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 7);
    auto pmsk           = _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1,  0);

    // 1.使用 lambda 将无法利用指令级并行，所以这里用宏
    // 2.clang 对指令并行重排序编译的效果要弱于 gnu 系列编译器，不过 clang 更适合自动优化而不是手动
    // 3.avx 不支持 ymm >> imm8 * 8 导致冗长的指令，但却支持奇葩的 ymm.xmm[0] >> imm8 * 8 | ymm.xmm[1] >> imm8 * 8
    // 4.avx _mm256_permutevar8x32_epi32 是第 3 点的替代方案，
    //   但由于必须要选一个元素参与洗牌操作而导致 pidx 指向的最后一个元素无处安放，而不得不通过 pmsk 屏蔽掉
    #define xgen(ptop,value)                                                       \
        auto pcur  ## ptop     = _mm256_set1_epi32(value);                         \
        auto pcmp  ## ptop     = _mm256_cmpgt_epi32(pcur ## ptop, ptop);           \
        auto pcmp0 ## ptop     = _mm256_permutevar8x32_epi32(pcmp ## ptop, pidx);  \
        auto pcmp1 ## ptop     = _mm256_and_si256(pcmp0 ## ptop, pmsk);            \
        auto pmin0 ## ptop     = _mm256_min_epi32(pcur ## ptop, ptop);             \
        auto pmin1 ## ptop     = _mm256_permutevar8x32_epi32(pmin0 ## ptop, pidx); \
        auto pmax  ## ptop     = _mm256_max_epi32(pcur ## ptop, ptop);             \
        auto pnew  ## ptop     = _mm256_blendv_ps(                                 \
            _mm256_castsi256_ps(pmax ## ptop),                                     \
            _mm256_castsi256_ps(pmin1 ## ptop),                                    \
            _mm256_castsi256_ps(pcmp1 ## ptop)                                     \
        );                                                                         \
        ptop                    = _mm256_castps_si256(pnew ## ptop);

    for(uxx i = 0; i < length / 2; i++){
        xgen(ptopa, ary[i * 2 + 0]); // 通道 a
        xgen(ptopb, ary[i * 2 + 1]); // 通道 b 这里只实现两个通道，由于 ALU 有限，再增加通道数是不会有性能提升的
    }

    for(uxx i = 0; i < 8; i++){
        xgen(ptopa, i32p(& ptopb)[i]);
    }
    if (length % 2 != 0){
        xgen(ptopa, ary[length - 1]);
    }
    _mm256_storeu_pd(f64p(k), _mm256_castsi256_pd(ptopa));
}

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
    auto pqf        = _mm256_floor_pd(pq);              // parallel floor
    auto pqi32      = _mm256_cvtpd_epi32(pqf);          // parallel quotient f64 -> i32
    auto pqi64      = _mm256_cvtepi32_epi64(pqi32);     // parallel quotient f64 -> i32
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
