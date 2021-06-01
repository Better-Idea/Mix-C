#ifndef xpack_math_align
#define xpack_math_align
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_align::inc
#include"define/base_type.hpp"
#include"instruction/index_of_last_set.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_align{
    // 让 size 以 2 的指数次方 向上圆整
    // 例如：
    // align(0x14) = 0x20
    // 注意：
    // align(0) = 0
    // align(v) = 1 if v > 2 exp (sizeof(uxx) * 8 - 1)
    template<class type_t>
    inline type_t align(type_t size){
        uxx i                           = inc::index_of_last_set(size);

        // 当 i == not_exist 时，mask = uxx(-1)，value 的值仍然是 0
        uxx mask                        = (uxx(1) << i) - 1;
        uxx value                       = (size + mask) & ~mask;
        return value;
    }

    // 让 size 的低 bits 位以 2 的指数次方 向上圆整
    // 例如：
    // align(0x123, 4) = 0x130
    template<class type_t>
    inline type_t align(type_t size, type_t bits){
        type_t mask = (type_t(1) << bits) - 1;
        return (size + mask) & ~mask;
    }
}

#endif

xexport(mixc::math_align::align)