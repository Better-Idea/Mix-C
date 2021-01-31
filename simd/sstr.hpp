/* simd string library (sstr)
 * 注意：
 * 需要保证越界内存仍可访问，否则可能访问到不在页表中的地址导致 CPU 产生缺页异常
 */

#pragma once
#include"define/base_type.hpp"

namespace mixc::simd{
    typedef struct {
        asciis  ptr;
        uxx     len;
    } sstr;

    extern ixx      compare(sstr left, sstr right);
    extern uxx      index_of_first(sstr seq, char value);
    extern uxx      index_of_last(sstr seq, char value);
    extern u32      to_u32(sstr seq);
    extern sstr     to_upper(sstr buf, sstr seq);
    extern sstr     to_lower(sstr buf, sstr seq);
}
