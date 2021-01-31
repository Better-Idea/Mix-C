#pragma push_macro("xuser")
#undef  xuser
#define xuser   mixc::simd::inc
#include"instruction/index_of_first_set.hpp"
#include"instruction/index_of_last_set.hpp"
#include"immintrin.h"
#include"math/min.hpp"
#include"memop/copy.hpp"
#include"simd/sstr.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::simd{
    constexpr u32 lut_base[] = { 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static inline sstr case_cast(sstr buf, sstr seq, char left_char, char right_char){
        auto left  = _mm256_set1_epi8(left_char);
        auto right = _mm256_set1_epi8(right_char);
        auto mask  = _mm256_set1_epi8(32); // 'a' - 'A' -> 32
        auto step  = sizeof(left);

        #define xcvt(mem_ptr,i)                                                             \
            auto __vals   = _mm256_loadu_si256((__m256i *)(seq.ptr + (i)));                 \
            auto __cmpl   = _mm256_cmpgt_epi8(__vals, left);                                \
            auto __cmpr   = _mm256_cmpgt_epi8(right, __vals);                               \
            auto __in     = _mm256_and_si256(__cmpl, __cmpr);                               \
            auto __cast   = _mm256_and_si256(__in, mask);                                   \
            auto __result = _mm256_xor_si256(__cast, __vals);                               \
            (mem_ptr)[0]    = __result;

        auto r     = buf;
        auto mem   = _mm256_setzero_si256();
        r.len      = seq.len;

        if (auto small = seq.len & (step - 1); small != 0) {
            xcvt(& mem, 0);
            inc::copy(u08p(buf.ptr), u08p(& mem), small);
            buf.ptr += small;
            seq.len -= small;
            seq.ptr += small;
        }

        for(uxx i = 0; i < seq.len; i += step){
            xcvt(& mem, i);
            _mm256_storeu_si256((__m256i *)(buf.ptr + i), mem);
        }

        #undef  xcvt
        return r;
    }

    extern ixx compare(sstr left, sstr right){
        uxx len = inc::min(left.len, right.len);

        for(uxx i = 0; i < len; i += sizeof(__m256i)){
            auto ymm0 = _mm256_loadu_si256((__m256i *)(left.ptr + i));
            auto ymm1 = _mm256_loadu_si256((__m256i *)(right.ptr + i));
            auto cmpr = _mm256_cmpeq_epi8(ymm0, ymm1);
            auto bits = _mm256_movemask_epi8(cmpr);

            if (bits = ~bits; bits == 0){
                continue;
            }
            if (auto m = inc::index_of_first_set(bits), ii = m + i; ii >= len){
                break;
            }
            else{
                return ixx(left.ptr[ii] - right.ptr[ii]);
            }
        }

        if (left.len == right.len){
            return 0;
        }
        return ixx(left.len - right.len);
    }

    extern uxx index_of_first(sstr seq, char value){
        auto ymm0  = _mm256_set1_epi8(value);
        auto step  = sizeof(ymm0);
        auto match = not_exist;
        uxx  i     = 0;

        for(; i < seq.len; i += step){
            auto ymm1 = _mm256_loadu_si256((__m256i *)(seq.ptr + i));
            auto cmpr = _mm256_cmpeq_epi8(ymm0, ymm1);
            auto bits = _mm256_movemask_epi8(cmpr);

            if (bits != 0){
                match = bits;
                break;
            }
        }

        if (match == not_exist){
            return not_exist;
        }
        if (uxx offset = inc::index_of_first_set(match); i + offset < seq.len){
            return i + offset;
        }
        else{
            return not_exist;
        }
    }

    extern uxx index_of_last(sstr seq, char value){
        auto ymm0  = _mm256_set1_epi8(value);
        auto step  = sizeof(ymm0);
        auto match = not_exist;
        auto end   = seq.ptr + seq.len;
        uxx  i     = 0;

        while(i < seq.len){
            i        += step;
            auto ymm1 = _mm256_loadu_si256((__m256i *)(end - i));
            auto cmpr = _mm256_cmpeq_epi8(ymm0, ymm1);
            auto bits = _mm256_movemask_epi8(cmpr);

            if (bits != 0){
                match = bits;
                break;
            }
        }

        if (match == not_exist){
            return not_exist;
        }
        if (uxx offset = 32 - inc::index_of_last_set(match), ii = seq.len - i - offset; ixx(ii) >= 0){
            return ii;
        }
        else{
            return not_exist;
        }
    }

    extern u32 to_u32(sstr seq){
        if (seq.ptr[0] == '+'){
            seq.ptr += 1;
            seq.len -= 1;
        }

        auto max_length = 10;
        auto offset     = max_length - seq.len;
        auto zero       = _mm256_set1_epi8('0');
        auto ymm0       = _mm256_loadu_si256((__m256i *)seq.ptr);
        auto ymm1       = _mm256_sub_epi8(ymm0, zero);
        auto full       = _mm256_cvtepi8_epi32(_mm256_extractf128_si256(ymm1, 0));
        auto muli       = _mm256_loadu_si256((__m256i *)(lut_base + offset));
        auto part       = _mm256_mullo_epi32(full, muli);
        auto sum0       = _mm_add_epi32(_mm256_extractf128_si256(part, 0), _mm256_extractf128_si256(part, 1));
        auto sum1       = _mm_hadd_epi32(sum0, sum0);
        auto sum2       = (sum1[0] >> 32) + (sum1[0] & 0xffffffff);
        
        if (offset == 0){
            sum2 += _mm256_extract_epi8(ymm1, 8) * 10 + _mm256_extract_epi8(ymm1, 9);
        }
        else if (offset == 1){
            sum2 += _mm256_extract_epi8(ymm1, 8);
        }
        return sum2;
    }

    extern sstr to_upper(sstr buf, sstr seq){
        return case_cast(buf, seq, 'a' - 1, 'z' + 1);
    }

    extern sstr to_lower(sstr buf, sstr seq){
        return case_cast(buf, seq, 'A' - 1, 'Z' + 1);
    }
}
