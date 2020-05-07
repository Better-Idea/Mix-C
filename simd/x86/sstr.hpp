/* simd string library (sstr)
 * 注意：
 * 需要保证越界内存仍可访问，否则可能访问到不在页表中的地址导致 CPU 产生缺页异常
 */

#pragma once
#pragma push_macro("xuser")
    #undef  xuser
    #define xuser   simd
    #include"define/base_type.hpp"
    #include"instruction/index_of_first_set.hpp"
    #include"instruction/index_of_last_set.hpp"
    #include"immintrin.h"
#pragma pop_macro("xuser")

#include"beginc"

struct sstr{
    asciis  ptr;
    uxx     len;
};

inline uxx sstr_index_of_first(sstr seq, char value){
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

inline uxx sstr_index_of_last(sstr seq, char value){
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

inline ixx sstr_compare(sstr left, sstr right){
    using namespace simd::inc;
    uxx r   = not_exist;
    uxx len = left.len <= right.len ? left.len : right.len;

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

#include"endc"
