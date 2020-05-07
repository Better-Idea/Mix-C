/* simd string library (sstr)
 */

#pragma once
#pragma push_macro("xuser")
    #undef  xuser
    #define xuser   simd
    #include"define/base_type.hpp"
    #include"instruction/index_of_first_set.hpp"
    #include"immintrin.h"
#pragma pop_macro("xuser")

#include"beginc"

struct sstr{
    asciis  ptr;
    uxx     len;
};

inline uxx sstr_index_of_first(sstr seq, char value){
    using namespace simd::inc;
    if (seq.ptr == nullptr){
        return not_exist;
    }

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

#include"endc"
