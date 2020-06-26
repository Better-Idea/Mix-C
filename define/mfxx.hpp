#ifndef xpack_define_mfxx
#define xpack_define_mfxx
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::define_mfxx
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::define_mfxx{
        template<
            class   float_type, 
            class   equivalent_type, 
            uxx     decimal_bits, 
            uxx     exp_bits, 
            uxx     exp_offset>
        struct mfxx{
            xgc_fields(
                xiam(mfxx<
                    float_type,
                    equivalent_type,
                    decimal_bits,
                    exp_bits,
                    exp_offset
                >)
            );
        public:
            union {
                struct {
                    equivalent_type decimal : decimal_bits;
                    equivalent_type exp     : exp_bits;
                    equivalent_type sign    : 1;
                };
                float_type value;

                struct {
                    operator ixx () const {
                        return ixx(the.exp) - exp_offset;
                    }

                    void operator = (ixx value) const {
                        the.exp = value + exp_offset;
                    }
                } real_exp;

                struct {
                    operator u64 () const {
                        return u64(1) << decimal_bits | the.decimal;
                    }
                } real_decimal;
            };

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

#endif

namespace xuser::inc{
    using ::mixc::define_mfxx::mf32;
    using ::mixc::define_mfxx::mf64;
}
