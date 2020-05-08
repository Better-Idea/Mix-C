/* simd string library (sstr)
 * 注意：
 * 需要保证越界内存仍可访问，否则可能访问到不在页表中的地址导致 CPU 产生缺页异常
 */

#pragma once
#include"beginc"
    struct sstr{
        asciis  ptr;
        uxx     len;
    };

    extern ixx      sstr_compare(sstr left, sstr right);
    extern uxx      sstr_index_of_first(sstr seq, char value);
    extern uxx      sstr_index_of_last(sstr seq, char value);
    extern u32      sstr_stou(sstr seq);
    extern sstr     sstr_to_upper(sstr buf, sstr seq);
    extern sstr     sstr_to_lower(sstr buf, sstr seq);
#include"endc"

