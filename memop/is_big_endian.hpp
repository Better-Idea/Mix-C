#ifndef xpack_memop_is_big_endian
#define xpack_memop_is_big_endian
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_is_big_endian::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_integer.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_is_big_endian{
    inline bool is_big_endian(){
        // 大端模式
        // 高位存放在低字节
        // 0xff ... fe
        uxx  v      = uxx(-1) << 1;
        bool test   = *u08p(& v) == 0xff;
        return test;
    }
}

#endif

xexport(mixc::memop_is_big_endian::is_big_endian)