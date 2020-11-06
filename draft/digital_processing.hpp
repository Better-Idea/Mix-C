#ifndef xpack_draft_digital_processing
#define xpack_draft_digital_processing
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_digital_processing::inc
#include"define/base_type.hpp"
#include"meta/is_same.hpp"
#include"math/const.hpp"
#include"math/min.hpp"
#include<intrin.h>

#pragma pop_macro("xuser")

namespace mixc::draft_digital_processing{
    template<class item_t>
    void triangular_wave(item_t * buffer, uxx length, uxx scale, uxx asymmetry) {
        // 构建定点数
        auto shift      = (sizeof(uxx) * 8 / 2);
        auto h          = (asymmetry) << shift;
        auto h2         = (h * 2);
        auto accumulate = (0);
        auto cycle      = (length / scale);
        auto step       = (h2 / scale);

        // TODO：不足一个周期的首部

        for(auto i = 0; i < cycle; i++, accumulate -= h2) {
            while(accumulate <= h) {
                buf[0]      = accumulate >> shift;
                buf        += 1;
                accumulate += step;
            }

            // 对称性
            while (accumulate <= h2) {
                buf[0]      = (h2 - accumulate) >> shift;
                buf        += 1;
                accumulate += step;
            }
        }

        // TODO：不足一个周期的尾部
    }

    template<class item_t>
    void vector_slope_core(item_t * buffer, uxx length, f64 offset, f64 slope){
        using namespace xuser;
        auto i                  = uxx(0);
        auto pk                 = _mm256_setr_pd(0.0, 1.0, 2.0, 3.0);
        auto ps                 = _mm256_set1_pd(slope);
        auto p4                 = _mm256_set1_pd(4.0);
        auto po                 = _mm256_set1_pd(offset);
        auto ps4                = _mm256_mul_pd(ps, p4);
        auto psk                = _mm256_fmadd_pd(ps, pk, po);
        auto topi               = _mm_set1_epi32(i32(max_value_of<i32>));
        auto step               = 4;

        if constexpr (is_same<u32, item_t>){
            psk                 = _mm256_sub_pd(
                psk, _mm256_set1_pd(f64(max_value_of<i32>))
            );
        }

        for(; i < length / step; i++, buffer += step){
            if constexpr (is_same<f64, item_t>){
                _mm256_storeu_pd(buffer, psk);
            }
            else if constexpr (is_same<f32, item_t>){
                _mm_storeu_ps(buffer, _mm256_cvtpd_ps(psk));
            }
            else if constexpr (is_same<u32, item_t>){
                auto pski       = _mm256_cvtpd_epi32(psk);
                auto adj        = _mm_add_epi32(pski, topi);
                _mm_storeu_pd(f64p(buffer), _mm_castsi128_pd(adj));
            }
            else{ // i32
                auto pski       = _mm256_cvtpd_epi32(psk);
                _mm_storeu_pd(f64p(buffer), _mm_castsi128_pd(pski));
            }
            psk = _mm256_add_pd(psk, ps4);
        }

        auto r                  = f64p(& psk);

        for(i *= step; i < length; buffer++, i++, r++){
            buffer[0]           = item_t(r[0] + f64(max_value_of<i32>));
        }
    }

    template<class item_t>
    void vector_slope_core(item_t * buffer, uxx length, f32 offset, f32 slope){
        using namespace xuser;
        auto pk                 = _mm256_setr_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
        auto ps                 = _mm256_set1_ps(slope);
        auto p8                 = _mm256_set1_ps(8.0);
        auto po                 = _mm256_set1_ps(offset);
        auto ps8                = _mm256_mul_ps(ps, p8);
        auto ps16               = _mm256_add_ps(ps8, ps8);
        auto psk0               = _mm256_fmadd_ps(ps, pk, po);
        auto psk1               = _mm256_add_ps(psk0, ps8);
        auto i                  = uxx(0);
        auto step               = uxx(16);
        auto sorted             = __m256i{};
        auto gen                = [&](){
            auto p0             = _mm256_cvtps_epi32(psk0);
            auto p1             = _mm256_cvtps_epi32(psk1);
            auto pack           = _mm256_packus_epi32(p0, p1);
            sorted              = _mm256_permute4x64_epi64(pack, 0 | 2 << 2 | 1 << 4 | 3 << 6);
        };
        auto rem                = (item_t *)& sorted;

        for(uxx j = 0; i < length / step; i++, buffer += step){
            if constexpr (is_same<u16, item_t>){
                gen();
                psk0            = _mm256_add_ps(psk0, ps16);
                psk1            = _mm256_add_ps(psk1, ps16);
                _mm256_storeu_pd(f64p(buffer), _mm256_castsi256_pd(sorted));
            }
            else{
                
            }
        }

        for(gen(), i *= step; i < length; buffer++, i++, rem++){
            buffer[0]           = rem[0];
        }
    }

    template<class item_t, class float_t>
    void vector_slope(item_t * buffer, uxx length, float_t offset, float_t slope){
        using namespace xuser;

        if (auto v = item_t(offset); slope == 0){
            for(uxx i = 0; i < length; i++){
                buffer[i] = v;
            }
            return;
        }

        auto limit              = slope > 0 ? max_value_of<item_t> : min_value_of<item_t>;
        auto limit_length       = uxx(f64(limit - offset) / slope);

        for(auto i = limit_length; i < length; i++){
            buffer[i] = limit;
        }

        vector_slope_core(buffer, min(limit_length, length), offset, slope);
    }
}

#endif
