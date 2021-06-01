#ifndef xpack_define_bits_bind
#define xpack_define_bits_bind
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_bits_bind::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_bits_bind{
    template<class type_t, class bit_t = bool>
    xstruct(
        xtmpl(bits_bind, type_t, bit_t),
        xprif(m_ptr,  type_t *),
        xprif(m_mask, type_t)
    )
        bits_bind() : bits_bind(nullptr, 0) {}
        bits_bind(type_t * bits, uxx index){
            the.bind(bits, index);
        }

        bit_t operator = (bit_t value){
            m_ptr[0]    = value ? m_ptr[0] | m_mask : m_ptr[0] & ~m_mask;
            return value;
        }

        void bind(type_t * bits, uxx index){
            m_ptr       = bits;
            m_mask      = type_t(1) << index;
        }

        void swap_value(bits_bind value){
            bit_t a     = the;
            bit_t b     = value;
            the         = b;
            value       = a;
        }

        operator bit_t(){
            return bit_t((m_ptr[0] & m_mask) != 0);
        }
    $
}

#endif

xexport(mixc::utils_bits_bind::bits_bind)