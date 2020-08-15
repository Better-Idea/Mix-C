#ifndef xpack_define_mfxx
#define xpack_define_mfxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_mfxx
#include"define/base_type.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_mfxx{
    template<
        class   float_type, 
        class   equivalent_type, 
        uxx     dec_bits, 
        uxx     exp_bits, 
        uxx     exp_offset,
        uxx     prec_dec>
    xstruct(
        xtmpl(mfxx,
            float_type,
            equivalent_type,
            dec_bits,
            exp_bits,
            exp_offset,
            prec_dec
        )
    )
        union{
            struct {
                equivalent_type decimal : dec_bits;
                equivalent_type exp     : exp_bits;
                equivalent_type sign    : 1;
            };

            float_type value;
        };

        the_t & real_exp(ixx value){
            the.exp = value + exp_offset;
            return the;
        }

        ixx real_exp_unsafe() const {
            return ixx(the.exp) - exp_offset;
        }

        ixx real_exp() const {
            if (the == 0){
                return 0;
            }
            return real_exp_unsafe();
        }

        // 注意：
        // f80 与 f64, f32 的小数部分不同，f80 没有隐藏位
        u64 real_dec_unsafe() const {
            if (not inc::is_same<float_type, f80>){
                return u64(u64(1) << dec_bits | the.decimal);
            }
            else{
                return u64(the.decimal);
            }
        }

        u64 real_dec() const {
            if (the == 0){
                return 0;
            }
            return real_dec_unsafe();
        }

        mfxx() : value(0) { }
        mfxx(float_type value) : value(value) { }

        constexpr mfxx(equivalent_type sign, equivalent_type exp, equivalent_type decimal) : 
            decimal(decimal), exp(exp), sign(sign) {}

        constexpr uxx decimal_bits(){
            return dec_bits;
        }

        constexpr uxx decimal_bits_full(){
            return dec_bits + not inc::is_same<float_type, f80>;
        }

        constexpr uxx precious(){
            return prec_dec;
        }

        operator float_type & () {
            return value;
        }

        operator const float_type & () const {
            return value;
        }
    $

    using mf32 = mixc::define_mfxx::mfxx<f32, u32, 23, 8 , (1 <<  8) - 1, 7 >;
    using mf64 = mixc::define_mfxx::mfxx<f64, u64, 52, 11, (1 << 10) - 1, 17>;
    using mf80 = mixc::define_mfxx::mfxx<f80, u64, 64, 15, (1 << 14) - 1, 19>;

    template<class> struct base;
    template<>      struct base<f32> : mf32{ using mf32::mfxx; };
    template<>      struct base<f64> : mf64{ using mf64::mfxx; };
    template<>      struct base<f80> : mf80{ using mf80::mfxx; };
}

namespace mixc::define_mfxx::origin{
    using mixc::define_mfxx::mf32;
    using mixc::define_mfxx::mf64;
    using mixc::define_mfxx::mf80;

    template<class float_t> 
    xstruct(
        xtmpl(mfxx, float_t), 
        xpubb(base<float_t>)
    )
        using base<float_t>::base;

        mfxx(float_t value) : 
            base<float_t>(value){
        }
    $
}

#endif

namespace xuser::inc{
    using namespace ::mixc::define_mfxx::origin;
}
