/* 模块：mfxx
 * 别名：mf32/mf64/mf80
 * 类型：多例·工具类
 * 功能：用于获取和设置浮点数的字段布局
 * 用法：
 * // 初始化，可以当普通浮点数用
 * // mf32 对应 f32
 * // mf64 对应 f64
 * // mf80 对应 f80
 * mf64 f    = 1.5;
 * 
 * // 获取原始尾数部分
 * u64  dec  = f.dec_bits;
 * 
 * // 获取原始指数部分
 * u64  exp  = f.exp_bits;
 * 
 * // 获取符号位
 * u64  sign = f.sign;
 * 
 * // 获取原始浮点值
 * f64  fo   = f.value;
 * 
 * 注意：
 * 原始的尾数部分不包含隐藏的 '1'（f32/f64 独有）
 * 原始指数部分并非实际的指数，还需要进行一步运算才可得到
 * 具体详情可搜索[浮点数]了解更多
 * 
 * TODO：==============================================================
 * - 介绍浮点数
 * - 浮点数的陷阱
 */

#ifndef xpack_utils_mfxx
#define xpack_utils_mfxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_mfxx::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_mfxx{
    template<
        class   float_t, 
        class   equivalent_t, 
        uxx     dec_bits_v, 
        uxx     exp_bits_v, 
        uxx     exp_offset_v,
        uxx     prec_dec_v>
    xstruct(
        xtmpl(mfxx,
            float_t,
            equivalent_t,
            dec_bits_v,
            exp_bits_v,
            exp_offset_v,
            prec_dec_v
        )
    )
        union{
            struct {
                equivalent_t decimal : dec_bits_v;
                equivalent_t exp     : exp_bits_v;
                equivalent_t sign    : 1;
            };

            float_t value;
        };

        constexpr mfxx() : value(0) { }
        constexpr mfxx(float_t value) : value(value) { }
        constexpr mfxx(equivalent_t sign, equivalent_t exp, equivalent_t decimal) : 
            decimal(decimal), exp(exp), sign(sign) {}

        // 函数：设置浮点数实际的指数
        // 参数：
        // - value 为新的指数值
        // 返回：当前对象的的引用
        // 注意：这里不考虑原始指数为 0 的情况（f32/f64 不使用隐藏位的情况）
        the_t & real_exp_unsafe(ixx value){
            the.exp = value + exp_offset_v;
            return the;
        }

        // 函数：设置浮点数实际的指数
        // 参数：
        // - value 为新的指数值
        // 返回：当前对象的的引用
        the_t & real_exp(ixx value){
            if constexpr (has_hidden_bit()){
                // 特殊情况，具有隐藏位的浮点数原始指数为 0 时不使用隐藏位
                // 此时 decimal 中存放所有有效位
                // 实数位（原来的隐藏位）放在最高位
                // 从不具有隐藏位转换到具有隐藏位
                if (the.exp == 0){
                    if (uxx(-value) != exp_offset_v){
                        the.decimal <<= 1;
                    }
                }
                // 从具有隐藏位转换到不具有隐藏位
                else if (uxx(-value) == exp_offset_v){
                    the.decimal = (the.decimal >> 1) | (u64(1) << (dec_bits_v - 1));
                }
            }

            real_exp_unsafe(value);
            return the;
        }

        // 函数：获取浮点数实际的指数[不带检查]
        // 注意：如果该浮点数值为 0，则会得到错误的指数部分
        ixx real_exp_unsafe() const {
            return ixx(the.exp) - exp_offset_v;
        }

        // 函数：获取浮点数实际的指数
        ixx real_exp() const {
            if (the == 0){
                return 0;
            }
            return real_exp_unsafe();
        }

        // 函数：获取浮点数实际的尾数部分[不带检查]
        // 注意：
        // - 如果该浮点数值为 0，则会得到错误的指数部分
        // - f80 与 f64, f32 的小数部分不同，f80 没有隐藏位
        u64 real_dec_unsafe() const {
            if constexpr (not inc::is_same<float_t, f80>){
                return u64(u64(1) << dec_bits_v | the.decimal);
            }
            else{
                return u64(the.decimal);
            }
        }

        // 函数：获取浮点数实际的尾数部分
        // 注意：
        // - f80 与 f64, f32 的小数部分不同，f80 没有隐藏位
        u64 real_dec() const {
            if (the == 0){
                return 0;
            }
            return real_dec_unsafe();
        }

        // 函数：尾数部分的位数
        static constexpr uxx decimal_bits(){
            return dec_bits_v;
        }

        // 函数：包括隐藏位的尾数部分的尾数
        // 注意：
        // - f80 与 f64, f32 的小数部分不同，f80 没有隐藏位
        static constexpr uxx decimal_bits_full(){
            return dec_bits_v + not inc::is_same<float_t, f80>;
        }

        static constexpr bool has_hidden_bit(){
            return not inc::is_same<float_t, f80>;
        }

        // 函数：该浮点数最大的精度（10进制）
        static constexpr uxx precious(){
            return prec_dec_v;
        }

        operator float_t & () {
            return value;
        }

        operator const float_t & () const {
            return value;
        }
    $

    using mf32 = mixc::utils_mfxx::mfxx<f32, u32, 23, 8 , (1 <<  7) - 1, 7 >;
    using mf64 = mixc::utils_mfxx::mfxx<f64, u64, 52, 11, (1 << 10) - 1, 17>;
    using mf80 = mixc::utils_mfxx::mfxx<f80, u64, 64, 15, (1 << 14) - 1, 19>;

    template<class> struct base;
    template<>      struct base<f32> : mf32{ using mf32::mfxx; };
    template<>      struct base<f64> : mf64{ using mf64::mfxx; };
    template<>      struct base<f80> : mf80{ using mf80::mfxx; };
}

namespace mixc::utils_mfxx::origin{
    using mixc::utils_mfxx::mf32;
    using mixc::utils_mfxx::mf64;
    using mixc::utils_mfxx::mf80;

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

xexport_space(mixc::utils_mfxx::origin)