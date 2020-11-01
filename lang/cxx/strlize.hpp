#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_strlize
#define xpack_lang_cxx_strlize
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_strlize::inc
#include"define/base_type.hpp"
#include"define/inf.hpp"
#include"define/mfxx.hpp"
#include"define/nan.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_callback.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx.hpp"
#include"math/min.hpp"
#include"math/max.hpp"
#include"math/numeration_t.hpp"
#include"math/exp10.hpp"
#include"math/expr10.hpp"
#include"math/const.hpp"
#include"memop/copy.hpp"
#include"memop/fill.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_strlize{
    constexpr char lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    constexpr char upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr bool is_scientific_notation = true;

    // Abstract：
    //                   precious
    //                   |||||||||
    //              real   decimal = max(0, precious - leading_zeros)
    //                ||   |||||
    // +1.23456e-3 -> +0.001234500
    //                   ||     ||
    //                   ||     trailing_zeros = precious - leading_zeros - decimal
    //                   leading_zeros
    //
    //               real    trailing_zeros = min(0, precious - rest)
    //               |||||   |||
    // +1.23456e3 -> +1234.56000
    //                     ||
    //                     rest = num_len - (exp + 1)
    //                     ||
    //                     decimal = min(precious, rest)
    //                     |||||
    //                     precious
    //
    //               real       precious
    //               |||||||    ||||
    // +1.23456e8 -> +123456000.0000
    //  | |||||             |||
    //  nul_len             expanding_zeros
    template<class item_t>
    using format_invoke = inc::can_callback<
        inc::cxx<item_t>(
            inc::cxx<item_t>        real,
            uxx                     expanding_zeros,
            uxx                     leading_zeros,
            inc::cxx<item_t>        decimal,
            uxx                     trailing_zeros,
            inc::cxx<item_t>        exp
        )
    >;

    enum class float_format_t{
        // 注意：不要改变枚举的顺序，下文依赖该顺序
        // 指数部分显示正负号 实数部分显示正负号
        // 0                  0                     fmt_1p2e3
        // 0                  1                     fmt_s1p2e3
        // 1                  0                     fmt_1p2es3
        // 1                  1                     fmt_s1p2es3
        fmt_1p2e3          , // "1.2e3"
        fmt_s1p2e3         , // "+1.2e3"
        fmt_1p2es3         , // "1.2e+3"
        fmt_s1p2es3        , // "+1.2e+3"
        fmt_1p2E3          , // "1.2E3"
        fmt_s1p2E3         , // "+1.2E3"
        fmt_1p2Es3         , // "1.2E+3"
        fmt_s1p2Es3        , // "+1.2E+3"
        fmt_n              , // "n"
        fmt_sn             , // "+n"
    };

    constexpr uxx force_real_part_sign  = uxx(float_format_t::fmt_s1p2e3);
    constexpr uxx force_exp_part_sign   = uxx(float_format_t::fmt_1p2es3);
    constexpr uxx force_upper_e         = uxx(float_format_t::fmt_1p2E3);

    enum class int_format_t{
        fmt_n               ,
        fmt_sn              ,
    };

    template<class item>
    struct core : inc::cxx<item> {
        using base_t = inc::cxx<item>;
        using base_t::base_t;
        using the_t = core<item>;

        core(base_t const & self) : 
            base_t(self){}

        template<class type>
        static auto strlize(type value, bool is_scientific_notation, uxx precious, format_invoke<item> invoke){
            #define xgen(v,str)                                         \
            if (value == v){                                            \
                the_t  real = value < 0 ? "-" str : "+" str;            \
                return invoke(real, 0, 0, "", 0, "");                   \
            }

            xgen(inc::nan, "nan")
            xgen(inc::inf, "inf")

            #undef  xgen

            item buf[64];
            item buf_exp[8]     = {'+'};
            auto pce            = ixx(precious);
            auto ptr            = buf;
            auto m              = inc::mfxx<type>{value};
            auto is_neg         = false;

            // 默认 precious 是 not_exist (max_value_of<uxx>)
            if (uxx(pce) > m.precious()){
                pce             = ixx(m.precious());
            }

            if (m < 0){
                m               = -m;
                ptr[0]          = '-';
                ptr            += 1;
                is_neg          = true;
            }
            else{
                ptr[0]          = '+';
                ptr            += 1;
            }

            auto num_part       = ptr;
            auto e              = inc::log10_2 * m.real_exp();
            auto exp            = ixx(e);
            auto is_neg_exp     = false;

            if (not is_scientific_notation){
                m              += inc::expr10_unsafe(pce) * 0.5;
            }

            // 以乘法代替除法
            if (exp < 0){
                is_neg_exp      = true;
                m              *= inc::exp10_unsafe(uxx(-exp));
                buf_exp[0]      = '-';
            }
            else if (exp > 0){
                m              *= inc::expr10_unsafe(uxx(exp));
            }

            if (m < 1.0){
                m              *= 10;
                exp            -= 1;
                buf_exp[0]      = '-';
            }
            else if (m >= 10){
                m              *= 0.1;
                exp            += 1;
            }

            if (is_scientific_notation){
                if (exp > pce){
                    m          += inc::exp10_unsafe(exp - pce) * 0.5;
                }
                else{
                    m          += inc::expr10_unsafe(pce - exp) * 0.5;
                }
            }

            if (exp < 0){
                exp = -exp;
            }

            auto exp_tmp        = exp;
            auto exp_str        = the_t(buf_exp, 4);
            buf_exp[1]          = item('0' + exp_tmp / 100);
            exp_tmp            %= 100;
            buf_exp[2]          = item('0' + exp_tmp / 10);
            exp_tmp            %= 10;
            buf_exp[3]          = item('0' + exp_tmp);

            union{
                struct{
                    u64 decimal : 52;
                    u64 digital : 4;
                };
                u64 full;
            }f;

            // 这里用 auto 就好了，m 可能是 mf32 或 mf64，所以返回值类型不是固定的
            auto rd             = m.real_dec();
            auto re             = m.real_exp();
            auto dec_bits       = uxx(0);
            f.full              = re > 0 ? rd << re : rd >> -re;

            do{
                ptr[0]          = item(f.digital + '0');
                ptr            += 1;
                dec_bits       += 1;
                f.digital       = 0;
                f.full         *= 10;
            } while(f.full != 0 and dec_bits < m.precious());

            if (is_scientific_notation){
                while(ptr > num_part + 1 and ptr[-1] == '0'){
                    ptr--;
                }
                auto ctz        = precious == not_exist ? ixx(0) : inc::max<ixx>(ixx(precious - dec_bits), 0);
                auto dec_len    = inc::min<uxx>(precious, uxx(ptr - num_part - 1));
                return invoke(the_t(buf, 1/*sign*/ + 1/*real*/), 0, 0, the_t(num_part + 1, dec_len), ctz, exp_str);
            }

            auto num_len        = ptr - num_part;

            if (is_neg_exp and exp != 0){
                auto real       = the_t(is_neg ? "-0" : "+0");
                auto decimal    = the_t(num_part, num_len);
                auto clz        = inc::min<uxx>(exp - 1, precious);
                auto ctz        = uxx(0);

                if (precious != not_exist){
                    decimal.length(
                        inc::min<uxx>(precious - clz, num_len)
                    );
                    ctz         = precious - clz - decimal.length();
                }
                return invoke(real, 0/*expanding_zeros*/, clz/*leading zeros*/, decimal, ctz/*trailing zeros*/, exp_str);
            }
            else{
                auto expand     = ixx(exp + 1/*real*/ - num_len);
                auto cez        = inc::max<ixx>(0, expand);
                auto rest       = inc::max<ixx>(0, -expand);
                auto dec_len    = inc::min<uxx>(precious, uxx(rest));
                auto real       = the_t(buf, exp + 1/*sign*/ + 1/*real*/ - cez);
                auto decimal    = the_t(real + real.length(), dec_len);
                auto clz        = 0;
                auto ctz        = precious == not_exist ? 0 : inc::max<ixx>(0, ixx(precious) - rest);
                return invoke(real, cez/*expanding_zeros*/, clz/*leading zeros*/, decimal, ctz/*trailing zeros*/, exp_str);
            }
        }

        template<class type>
        static auto strlize(
            type                    value,
            float_format_t          modes, 
            uxx                     precious, 
            inc::can_alloc<item>    alloc){

            auto mode   = uxx(modes);
            auto with_e = uxx(mode) >= uxx(float_format_t::fmt_n) ? 
                not is_scientific_notation: is_scientific_notation;

            return strlize(value, with_e, precious, [&](
                base_t              real,
                uxx                 expanding_zeros,
                uxx                 leading_zeros,
                base_t              decimal,
                uxx                 trailing_zeros,
                base_t              exp
            ) {

                // 不强制使用实数部分正负号
                if (not (mode & force_real_part_sign) and real[0] == '+'){
                    real = real.backward(1);
                }

                // 一般记数法
                if (not with_e){
                    auto dec_len    = decimal.length() + trailing_zeros;

                    if (dec_len != 0){
                        dec_len    += 1; // dot
                    }

                    auto len        = real.length() + expanding_zeros + leading_zeros + dec_len;
                    auto mem        = alloc(len);
                    auto ptr        = mem;

                    inc::copy_with_operator_unsafe(mem, real, real.length());
                    mem            += real.length();
                    inc::fill_with_operator(mem, '0', expanding_zeros);
                    mem            += expanding_zeros;

                    if (dec_len == 0){
                        return base_t(ptr, len);
                    }

                    mem[0]          = '.';
                    mem            += 1;
                    inc::fill_with_operator(mem, '0', leading_zeros);
                    mem            += leading_zeros;
                    inc::copy_with_operator_unsafe(mem, decimal, decimal.length());
                    mem            += decimal.length();
                    inc::fill_with_operator(mem, '0', trailing_zeros);
                    return base_t(ptr, len);
                }

                // 不强制使用指数部分的正负号
                if (not (mode & force_exp_part_sign) and exp[0] == '+'){
                    exp             = exp.backward(1);
                }

                auto dec_len        = decimal.length() == 0 ?
                    0 : 1/*dot*/ +  decimal.length();
                auto e_len          = exp.length() == 0 ?
                    0 : 1/*e*/ + exp.length();
                auto e              = mode & force_upper_e ? 'E' : 'e';
                auto len            = real.length() + dec_len + trailing_zeros + e_len;
                auto mem            = alloc(len);
                auto ptr            = mem;

                inc::copy_with_operator_unsafe(mem, real, real.length());
                mem                += real.length();

                if (e_len == 0){
                    return base_t(ptr, len);
                }
                if (dec_len != 0){
                    mem[0]         = '.';
                    mem           += 1;
                }

                inc::copy_with_operator_unsafe(mem, decimal, decimal.length());
                mem                += decimal.length();
                inc::fill_with_operator(mem, '0', trailing_zeros);
                mem                += trailing_zeros;
                mem[0]              = e;
                mem                += 1;
                inc::copy_with_operator_unsafe(mem, exp, exp.length());
                return base_t(ptr, len);
            });
        }

        template<class type>
        auto strlize(type value, int_format_t mode, uxx base, asciis lut, inc::can_alloc<item> alloc) const {
            u08  buf[64];
            u08p ptr = buf;
            using unsigned_t = inc::unsigned_type<type>;
            auto u           = unsigned_t(value);
            auto is_neg      = value < 0;

            if (is_neg){
                u = unsigned_t(0) - u;
            }

            do {
                ptr[0] = lut[u % base];
                ptr   += 1;
                u     /= unsigned_t(base);
            } while(u != 0);

            uxx    len = ptr - buf;
            item * mem = alloc(len + is_neg);
            the_t  r   = the_t(mem, len + is_neg);

            if (is_neg){
                mem[0] = '-';
                mem   += 1;
            }
            else if (mode == int_format_t::fmt_sn){
                mem[0] = '+';
                mem   += 1;
            }

            for(uxx i = 0; i < len; i++){
                mem[i] = buf[len - i - 1];
            }
            return r;
        }
    };

    template<class final, class base, class item>
    struct meta : base{
        using base::base;
        using the_t = core<item>;

        #define xgen(type)                                                                      \
        meta(type value, inc::can_alloc<item> alloc) :                                          \
            meta(value, int_format_t::fmt_n, inc::numeration_t::dec, lower, alloc){             \
        }                                                                                       \
                                                                                                \
        meta(type value, asciis lut, inc::can_alloc<item> alloc) :                              \
            meta(value, int_format_t::fmt_n, inc::numeration_t::dec, lut, alloc){               \
        }                                                                                       \
                                                                                                \
        meta(type value, inc::numeration_t radix, inc::can_alloc<item> alloc) :                 \
            meta(value, int_format_t::fmt_n, radix, lower, alloc){                              \
        }                                                                                       \
                                                                                                \
        meta(type value, inc::numeration_t radix, asciis lut, inc::can_alloc<item> alloc) :     \
            meta(value, int_format_t::fmt_n, radix, lower, alloc){                              \
        }                                                                                       \
                                                                                                \
        meta(type value, int_format_t fmt,                                                      \
            inc::numeration_t radix, asciis lut, inc::can_alloc<item> alloc){                   \
            thex = the.strlize(value, fmt, type(radix), lut, alloc);                            \
        }

        xgen(u08);
        xgen(u16);
        xgen(u32);
        xgen(u64);
        xgen(i08);
        xgen(i16);
        xgen(i32);
        xgen(i64);
        #undef  xgen

        #define xgen(type)                                                                                      \
        meta(type value, inc::can_alloc<item> alloc){                                                           \
            thex = the.template strlize<type>(value, float_format_t::fmt_1p2e3, not_exist, alloc);              \
        }                                                                                                       \
                                                                                                                \
        meta(type value, float_format_t mode, inc::can_alloc<item> alloc){                                      \
            thex = the.template strlize<type>(value, mode, not_exist, alloc);                                   \
        }                                                                                                       \
                                                                                                                \
        meta(type value, uxx precious, inc::can_alloc<item> alloc){                                  \
            thex = the.template strlize<type>(value, float_format_t::fmt_1p2e3, precious, alloc);    \
        }                                                                                                       \
                                                                                                                \
        meta(type value, float_format_t mode, uxx precious, inc::can_alloc<item> alloc){             \
            thex = the.template strlize<type>(value, mode, precious, alloc);                         \
        }

        xgen(f32)
        xgen(f64)
        xgen(f80)
        #undef  xgen

        meta(final value, inc::can_alloc<item> alloc){
            thex = the_t(value).clone(alloc);
        }

        meta(item value, inc::can_alloc<item> alloc){
            item * ptr = alloc(1);
            thex = the_t{ ptr, 1 };
            ptr[0] = value;
        }
    };
}

#endif

namespace mixc::lang_cxx_strlize::xuser{
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

namespace xuser{
    using ::mixc::lang_cxx_strlize::lower;
    using ::mixc::lang_cxx_strlize::upper;
    using ::mixc::lang_cxx_strlize::float_format_t;
    using ::mixc::lang_cxx_strlize::int_format_t;
}

#include"math/numeration_t.hpp"
#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strlize::xuser
