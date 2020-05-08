#pragma push_macro("xuser")
    #undef  xuser
    #define xuser   simd
    #include"define/base_type.hpp"
    #include"instruction/index_of_first_set.hpp"
    #include"instruction/index_of_last_set.hpp"
    #include"immintrin.h"
    #include"math/smaller.hpp"
    #include"memop/copy.hpp"
    #include"simd/x86/sstr.hpp"
#pragma pop_macro("xuser")

namespace mixc::simd_x86_sstr{
    static u32 base[] = { 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    inline sstr sstr_case_cast(sstr buf, sstr seq, char left_char, char right_char){
        using namespace simd::inc;
        auto left  = _mm256_set1_epi8(left_char);
        auto right = _mm256_set1_epi8(right_char);
        auto mask  = _mm256_set1_epi8(32); // 'a' - 'A' -> 32
        auto step  = sizeof(left);
        auto cvt   = [&](uxx i) {
            auto vals   = _mm256_loadu_si256((__m256i *)(seq.ptr + i));
            auto cmpl   = _mm256_cmpgt_epi8(vals, left);
            auto cmpr   = _mm256_cmpgt_epi8(right, vals);
            auto in     = _mm256_and_si256(cmpl, cmpr);
            auto cast   = _mm256_and_si256(in, mask);
            auto result = _mm256_xor_si256(cast, vals);
            return result;
        };

        auto r     = buf;
        r.len      = seq.len;

        if (auto small = seq.len & (step - 1); small != 0) {
            auto mem = cvt(0);
            copy(u08p(buf.ptr), u08p(& mem), small);
            buf.ptr += small;
            seq.len -= small;
            seq.ptr += small;
        }

        for(uxx i = 0; i < seq.len; i += step){
            _mm256_storeu_si256((__m256i *)(buf.ptr + i), cvt(i));
        }
        return r;
    }
}

#include"beginc"
ixx sstr_compare(sstr left, sstr right){
    using namespace simd::inc;
    uxx len = smaller(left.len, right.len);

    for(uxx i = 0; i < len; i += sizeof(__m256i)){
        auto ymm0 = _mm256_loadu_si256((__m256i *)(left.ptr + i));
        auto ymm1 = _mm256_loadu_si256((__m256i *)(right.ptr + i));
        auto cmpr = _mm256_cmpeq_epi8(ymm0, ymm1);
        auto bits = _mm256_movemask_epi8(cmpr);

        if (bits = ~bits; bits == 0){
            continue;
        }
        if (auto m = index_of_first_set(bits), ii = m + i; ii >= len){
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

uxx sstr_index_of_first(sstr seq, char value){
    using namespace simd::inc;

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
    if (uxx offset = index_of_first_set(match); i + offset < seq.len){
        return i + offset;
    }
    else{
        return not_exist;
    }
}

uxx sstr_index_of_last(sstr seq, char value){
    using namespace simd::inc;

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
    if (uxx offset = 32 - index_of_last_set(match), ii = seq.len - i - offset; ixx(ii) >= 0){
        return ii;
    }
    else{
        return not_exist;
    }
}

u32 sstr_stou(sstr seq){
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
    auto muli       = _mm256_loadu_si256((__m256i *)(mixc::simd_x86_sstr::base + offset));
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

sstr sstr_to_upper(sstr buf, sstr seq){
    return mixc::simd_x86_sstr::sstr_case_cast(buf, seq, 'a' - 1, 'z' + 1);
}

sstr sstr_to_lower(sstr buf, sstr seq){
    return mixc::simd_x86_sstr::sstr_case_cast(buf, seq, 'A' - 1, 'Z' + 1);
}

#include"endc"
