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
        xgc(mfxx, 
            xtmpl(
                float_type, 
                equivalent_type, 
                decimal_bits, 
                exp_bits, 
                exp_offset
            )
        )
            using mfxxp = mfxx<
                float_type, 
                equivalent_type, 
                decimal_bits, 
                exp_bits, 
                exp_offset
            >;

            union {
                struct {
                    equivalent_type decimal : decimal_bits;
                    equivalent_type exp     : exp_bits;
                    equivalent_type sign    : 1;
                };
                float_type value;

                struct {
                    operator ixx () const {
                        return ixx(mfxxp(this)->exp) - exp_offset;
                    }
                } real_exp;
            };

            xgc_fields();

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
        xgc_end();

        using mf32 = mfxx<f32, u32, 23, 8, 127 >;
        using mf64 = mfxx<f64, u64, 52, 11, 1023>;

        constexpr mf64 inf_pos { 0ull, 0x7ffull, 0ull };
        constexpr mf64 inf_neg { 1ull, 0x7ffull, 0ull };
    }

#endif

namespace xuser::inc{
    using mixc::define_mfxx::mf32;
    using mixc::define_mfxx::mf64;
    using mixc::define_mfxx::inf_pos;
    using mixc::define_mfxx::inf_neg;
}
