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

        u64 real_dec_unsafe() const {
            return u64(u64(1) << dec_bits | the.decimal);
        }

        // 对于 f80 需要用扩展精度的整数
        // TODO:=========================================================
        u64 real_dec() const {
            static_assert(not inc::is_same<float_type, f80>, "TODO: mfxx::real_dec need more bits integer");

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
            return dec_bits + 1;
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
}

namespace mixc::define_mfxx::origin{
    using mf32 = mixc::define_mfxx::mfxx<f32, u32, 23, 8 , (1 <<  8) - 1, 7 >;
    using mf64 = mixc::define_mfxx::mfxx<f64, u64, 52, 11, (1 << 10) - 1, 16>;
    using mf80 = mixc::define_mfxx::mfxx<f80, u64, 64, 15, (1 << 14) - 1, 18>;

    template<class> struct mfxx;
    template<> 
    xstruct(
        xspec(mfxx, f32), xpubb(mf32)
    )
        using mf32::mf32;

        mfxx(f32 value){
            the.value = value;
        }
    $
    
    template<> 
    xstruct(
        xspec(mfxx, f64), xpubb(mf64)
    )
        using mf64::mf64;

        mfxx(f64 value){
            the.value = value;
        }
    $
    
    template<> 
    xstruct(
        xspec(mfxx, f80), xpubb(mf80)
    )
        using mf80::mf80;

        mfxx(f80 value){
            the.value = value;
        }
    $
}

#endif

namespace xuser::inc{
    using namespace ::mixc::define_mfxx::origin;
}
