#pragma once
#include"define/base_type.hpp"

namespace mixc{
    // 把type转换成指定长度的无符号整数指针
    template<class type>
    inline auto base_seqlize(type const & value){
        #define gen(type,scale,...)                                 \
            if constexpr (sizeof(value) % scale == 0 __VA_ARGS__){  \
                struct seg{                                         \
                    type * ptr;                                     \
                    uxx    length;                                  \
                } r { (type *)& value, sizeof(value) / scale };     \
                return r;                                           \
            }
        
        gen(u64, 8);
        gen(u32, 4);
        gen(u16, 2);
        gen(u08, 1, || true);
        #undef gen
    }
}

