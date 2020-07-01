#ifndef xpack_define_mfxx
#define xpack_define_mfxx
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::define_mfxx
    #include"define/base_type.hpp"
    #include"macro/xgc.hpp"
    
    namespace xuser::origin{
        template<
            class   float_type, 
            class   equivalent_type, 
            uxx     decimal_bits, 
            uxx     exp_bits, 
            uxx     exp_offset>
        union mfxx{
            using the_t = mfxx<
                float_type,
                equivalent_type,
                decimal_bits,
                exp_bits,
                exp_offset
            >;

            struct {
                equivalent_type decimal : decimal_bits;
                equivalent_type exp     : exp_bits;
                equivalent_type sign    : 1;
            };

            float_type value;

            the_t & real_exp(ixx value){
                the.exp = value + exp_offset;
            }

            ixx real_exp() const {
                return ixx(the.exp) - exp_offset;
            }

            ixx real_dec() const {
                return u64(1) << decimal_bits | the.decimal;
            }

            mfxx() : value(0) { }
            mfxx(float_type value) : value(value) { }

            constexpr mfxx(equivalent_type sign, equivalent_type exp, equivalent_type decimal) : 
                sign(sign), exp(exp), decimal(decimal) {}

            operator float_type & () {
                return value;
            }

            operator const float_type & () const {
                return value;
            }
        };

        using mf32 = mfxx<f32, u32, 23, 8, 127 >;
        using mf64 = mfxx<f64, u64, 52, 11, 1023>;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::define_mfxx::origin;
}
