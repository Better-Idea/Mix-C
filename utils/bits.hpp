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
    template<class type, class bit_type = bool>
    xstruct(
        xtmpl(bits, type),
        xprif(data, type)
    )
        bits() : bits(0){}
        bits(type const & value) : data(value){}

        inc::bits_bind<type, bit_type> operator [](uxx index) const {
            return inc::bits_bind<type, bit_type>(xref data, index);
        }

        uxx length() const {
            return sizeof(type) * 8;
        }

        operator type & () const {
            return data;
        }
    $

    using b08 = bits<u08>;
    using b16 = bits<u16>;
    using b32 = bits<u32>;
    using b64 = bits<u64>;
}

#endif

xexport_space(mixc::utils_bits::origin)