#ifndef xpack_memop_bytes_load
#define xpack_memop_bytes_load
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_bytes_load::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_integer.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_bytes_load{
    template<inc::is_integer int_t>
    inline int_t bytes_load(const void * buffer, uxx bytes = sizeof(int_t)){
        cu08p ptr       = cu08p(buffer);
        int_t result    = 0;

        for(uxx i = 0, shift = (sizeof(int_t) - 1) * 8; i < bytes; i++, shift -= 8){
            result     |= int_t(ptr[i]) << shift;
        }
        return result;
    }
}

#endif

xexport(mixc::memop_bytes_load::bytes_load)