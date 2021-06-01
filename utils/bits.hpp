#ifndef xpack_define_bits
#define xpack_define_bits
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_bits::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"utils/bits_bind.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_bits::origin{
    template<class type_t, class bit_t = bool>
    xstruct(
        xtmpl(bits, type_t),
        xprif(m_value, type_t)
    )
        constexpr bits() : bits(0){}
        constexpr bits(type_t const & value) : m_value(value){}

        inc::bits_bind<type_t, bit_t> operator [](uxx index) const {
            return inc::bits_bind<type_t, bit_t>(xref m_value, index);
        }

        uxx length() const {
            return sizeof(type_t) * 8;
        }

        operator type_t & () const {
            return m_value;
        }
    $

    using b08 = bits<u08>;
    using b16 = bits<u16>;
    using b32 = bits<u32>;
    using b64 = bits<u64>;
}

#endif

xexport_space(mixc::utils_bits::origin)