#pragma once
#include"define/base_type.hpp"

namespace mixc{
    template<class float_type, class equivalent_type, uxx decimal_bits, uxx exp_bits, uxx exp_offset>
    union mfxx {
        typedef mfxx<float_type, equivalent_type, decimal_bits, exp_bits, exp_offset> * mfxxp;
        struct {
            equivalent_type decimal : decimal_bits;
            equivalent_type exp : exp_bits;
            equivalent_type sign : 1;
        };

        float_type value;

        struct {
            operator ixx () const {
                return ixx(mfxxp(this)->exp) - exp_offset;
            }
        } real_exp;

        mfxx() : value(0) { }
        mfxx(float_type value) : value(value) { }

        constexpr mfxx(equivalent_type sign, equivalent_type exp, equivalent_type decimal) : 
            sign(sign), exp(exp), decimal(decimal) {}

        operator float_type () const {
            return value;
        }

        operator float_type & () const {
            return value;
        }
    };
    using mf32  = mixc::mfxx<f32, u32, 23, 8, 127 >;
    using mf64  = mixc::mfxx<f64, u64, 52, 11, 1023>;

    constexpr mf64                      pos_inf { 0ull, 0x7ffull, 0ull };
    constexpr mf64                      neg_inf { 1ull, 0x7ffull, 0ull };
}

using mf32  = mixc::mf32;
using mf64  = mixc::mf64;

