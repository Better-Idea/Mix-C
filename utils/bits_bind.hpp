#ifndef xpack_define_bits_bind
#define xpack_define_bits_bind
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_bits_bind::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_bits_bind{
    template<class type_t, class bit_t = bool>
    xstruct(
        xtmpl(bits_bind, type_t, bit_t),
        xprif(ptr,  type_t *),
        xprif(mask, type_t)
    )
        bits_bind() : bits_bind(nullptr, 0) {}
        bits_bind(type_t * bits, uxx index){
            the.bind(bits, index);
        }

        bit_t operator = (bit_t value){
            ptr[0]      = value ? ptr[0] | mask : ptr[0] & ~mask;
            return value;
        }

        void bind(type_t * bits, uxx index){
            ptr         = bits;
            mask        = type_t(1) << index;
        }

        void swap_value(bits_bind value){
            bit_t a     = the;
            bit_t b     = value;
            the         = b;
            value       = a;
        }

        operator bit_t(){
            return bit_t((ptr[0] & mask) != 0);
        }
    $
}

#endif

xexport(mixc::utils_bits_bind::bits_bind)