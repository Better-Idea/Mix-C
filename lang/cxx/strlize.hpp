#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_strlize
#define xpack_lang_cxx_strlize
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_strlize
    #include"define/base_type.hpp"
    #include"define/inf.hpp"
    #include"define/mfxx.hpp"
    #include"define/nan.hpp"
    #include"interface/can_alloc.hpp"
    #include"lang/cxx/clone.hpp"
    #include"lang/cxx.hpp"
    #include"math/numeration_t.hpp"
    #include"math/exp10.hpp"
    #include"math/expr10.hpp"
    #include"math/const.hpp"
    #include"memop/copy.hpp"
    #include"memop/fill.hpp"
    #include"meta/more_fit.hpp"
    #include"meta/unsigned_type.hpp"

    namespace xuser{
        constexpr char lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
        constexpr char upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
            auto strlize(type value, float_format_t mode, uxx origin_number_of_significa, inc::can_alloc<item> alloc) const {
                #define xgen(v,str)                     \
                if (value == v){                        \
                    auto len = sizeof(str) - 1;         \
                    auto mem = alloc(len);              \
                    inc::copy(mem, str, len);           \
                    return the_t(mem, len);             \
                }

                uxx precious        = origin_number_of_significa;

                if (origin_number_of_significa > 16){
                    precious        = 16;
                }

                xgen(inc::nan, "nan")
                xgen(inc::inf_pos, "+inf")
                xgen(inc::inf_neg, "-inf")
                xgen(0, "0")
                #undef  xgen

                char buf[32];
                auto ptr            = buf;
                auto m              = inc::mf64(value);
                auto is_neg         = false;

                if (value < 0){
                    value           = -value;
                    ptr[0]          = '-';
                    ptr            += 1;
                    is_neg          = true;
                    m               = inc::mf64(value);
                }
                else if (uxx(mode) & uxx(float_format_t::fmt_s1p2e3)){
                    ptr[0]          = '+';
                    ptr            += 1;
                }

                auto num_part       = ptr;
                auto e              = inc::log10_2 * m.real_exp();
                auto exp            = ixx(e);
                auto is_neg_exp     = false;

                // 以乘法代替除法
                // 此处不能用 exp <= 0，因为后续转换中假定整数部分为 0
                if (exp < 0){ 
                    exp             = -exp;
                    is_neg_exp      = true;
                    m              *= inc::exp10(uxx(exp));

                    if (m < 1.0){
                        m          *= 10;
                        exp        += 1;
                    }
                }
                // 1/10^exp
                else{
                    m              *= inc::expr10(uxx(exp));

                    if (m >= 10){
                        m          *= 0.1;
                        exp        += 1;
                    }
                }

                if (origin_number_of_significa != not_exist){
                    auto plus       = inc::expr10(uxx(precious)) * 5.00000000005;
                    m              += plus;
                }

                union{
                    struct{
                        u64 decimal : 52;
                        u64 digital : 4;
                    };
                    u64 full;
                }f;

                auto rd             = m.real_dec();
                auto re             = m.real_exp();
                auto i              = 0;
                f.full              = re > 0 ? rd << re : rd >> -re;

                do{
                    ptr[0]          = f.digital + '0';
                    ptr            += 1;
                    i              += 1;
                    f.digital       = 0;
                    f.full         *= 10;
                } while(f.decimal != 0 and i < precious);

                if (origin_number_of_significa == not_exist){
                    while(ptr[-1] == '0'){
                        ptr        -= 1;
                        i          -= 1;
                    }
                }

                // 一般计数法
                if (uxx(mode) >= uxx(float_format_t::fmt_n)){
                    if (is_neg_exp){
                        auto length = (ptr - buf) + exp + 1/*dot*/;
                        auto mem    = alloc(length);
                        auto tmp    = mem;

                        // 复制正负号
                        inc::copy_with_operator(mem, buf, num_part - buf);
                        mem        += num_part - buf;
                        mem[0]      = '0';
                        mem[1]      = '.';
                        mem        += 2;
                        inc::fill_with_operator(mem, '0', exp - 1);
                        mem        += exp - 1;
                        inc::copy_with_operator(mem, num_part, ptr - num_part);
                        return the_t(tmp, length);
                    }

                    auto has_dot    = exp + 1 < i;
                    auto plus       = has_dot ? uxx(0) : uxx(exp + 1 - i);
                    auto digi_len   = has_dot ? exp + 1 : i;
                    auto length     = (ptr - buf) + plus + uxx(has_dot);
                    auto mem        = alloc(length);
                    auto tmp        = mem;

                    inc::copy_with_operator(mem, buf, num_part - buf);
                    mem            += num_part - buf;
                    inc::copy_with_operator(mem, num_part, digi_len);
                    mem            += digi_len;
                    num_part       += digi_len;

                    if (has_dot){
                        mem[0]      = '.';
                        mem        += 1;
                        inc::copy_with_operator(mem, num_part, ptr - num_part);
                    }
                    else{
                        inc::fill_with_operator(mem, '0', plus);
                    }
                    return the_t(tmp, length);
                }

                // 最多 3 位指数
                // 科学计数法
                if (uxx i = 0; exp != 0){
                    char tmp[3];
                    ptr[0]          = uxx(mode) & uxx(float_format_t::fmt_1p2E3) ? 'E' : 'e';
                    ptr            += 1;
                    tmp[0]          = exp / 100;
                    exp            %= 100;
                    tmp[1]          = exp / 10;
                    exp            %= 10;
                    tmp[2]          = exp;

                    while(tmp[i] == 0){
                        i += 1;
                    }
                    if (is_neg_exp){
                        ptr[0]      = '-';
                        ptr        += 1;
                    }
                    else if (uxx(mode) & uxx(float_format_t::fmt_1p2es3)){
                        ptr[0]      = '+';
                        ptr        += 1;
                    }
                    while(i < 3){
                        ptr[0] = tmp[i] + '0';
                        ptr        += 1;
                        i          += 1;
                    }
                }

                auto length         = (ptr - buf) + 1/*dot*/;
                auto mem            = alloc(length);
                auto tmp            = mem;

                inc::copy_with_operator(mem, buf, num_part - buf);
                mem                += num_part - buf;
                mem[0]              = num_part[0];
                mem                += 1;
                num_part           += 1;
                mem[0]              = '.';
                mem                += 1;
                inc::copy_with_operator(mem, num_part, ptr - num_part);
                return the_t(tmp, length);
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
                    u     /= base;
                } while(u != 0);

                auto len = ptr - buf;
                auto mem = alloc(len + is_neg);
                auto r   = the_t(mem, len + is_neg);

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
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_strlize::xuser{
    using ::mixc::lang_cxx_strlize::lower;
    using ::mixc::lang_cxx_strlize::upper;
    using ::mixc::lang_cxx_strlize::float_format_t;
    using ::mixc::lang_cxx_strlize::int_format_t;

    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        #define xgen(type)                                                                      \
        cxx(type value, inc::can_alloc<item> alloc) :                                           \
            cxx(value, int_format_t::fmt_n, inc::numeration_t::dec, lower, alloc){              \
        }                                                                                       \
                                                                                                \
        cxx(type value, asciis lut, inc::can_alloc<item> alloc) :                               \
            cxx(value, int_format_t::fmt_n, inc::numeration_t::dec, lut, alloc){                \
        }                                                                                       \
                                                                                                \
        cxx(type value, inc::numeration_t base, inc::can_alloc<item> alloc) :                   \
            cxx(value, int_format_t::fmt_n,  base, lower, alloc){                               \
        }                                                                                       \
                                                                                                \
        cxx(type value, inc::numeration_t base, asciis lut, inc::can_alloc<item> alloc){        \
            thex = the.strlize(value, int_format_t::fmt_n, type(base), lut, alloc);             \
        }

        xgen(u08);
        xgen(u16);
        xgen(u32);
        xgen(u64);
        xgen(i08);
        xgen(i16);
        xgen(i32);
        xgen(i64);

        cxx(f64 value, inc::can_alloc<item> alloc){
            thex = the.template strlize<f64>(value, float_format_t::fmt_1p2e3, not_exist, alloc);
        }

        cxx(f64 value, float_format_t mode, inc::can_alloc<item> alloc){
            thex = the.template strlize<f64>(value, mode, not_exist, alloc);
        }

        cxx(f64 value, uxx number_of_significa, inc::can_alloc<item> alloc){
            thex = the.template strlize<f64>(value, float_format_t::fmt_1p2e3, number_of_significa, alloc);
        }

        cxx(f64 value, float_format_t mode, uxx number_of_significa, inc::can_alloc<item> alloc){
            thex = the.template strlize<f64>(value, mode, number_of_significa, alloc);
        }

        cxx(final value, inc::can_alloc<item> alloc){
            thex = the_t(value).clone(alloc);
        }

        cxx(item value, inc::can_alloc<item> alloc){
            item * ptr = alloc(1);
            thex = the_t{ ptr, 1 };
            ptr[0] = value;
        }

        #undef  xgen
    };
}

namespace xuser::inc{
    using ::mixc::lang_cxx_strlize::lower;
    using ::mixc::lang_cxx_strlize::upper;
    using ::mixc::lang_cxx_strlize::float_format_t;
}

#include"math/numeration_t.hpp"
#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strlize::xuser
