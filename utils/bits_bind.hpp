#ifndef xpack_define_bits_bind
#define xpack_define_bits_bind
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_bits_bind::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_bits_bind{
    template<class type, class bit_type = bool>
    xstruct(
        xtmpl(bits_bind, type, bit_type),
        xprif(ptr,  type *),
        xprif(mask, type)
    )
        bits_bind() : bits_bind(nullptr, 0) {}
        bits_bind(type * bits, uxx index){
            the.bind(bits, index);
        }

        bit_type operator = (bit_type value){
            ptr[0]      = value ? ptr[0] | mask : ptr[0] & ~mask;
            return value;
        }

        void bind(type * bits, uxx index){
            ptr         = bits;
            mask        = type(1) << index;
        }

        void swap_value(bits_bind value){
            bit_type a  = the;
            bit_type b  = value;
            the         = b;
            value       = a;
        }

        operator bit_type(){
            return bit_type((ptr[0] & mask) != 0);
        }
    $
}

#endif

xexport(mixc::utils_bits_bind::bits_bind)