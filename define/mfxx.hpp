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

#ifndef xpack_define_mfxx
#define xpack_define_mfxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_mfxx::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
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

        mfxx() : value(0) { }
        mfxx(float_type value) : value(value) { }

        constexpr mfxx(equivalent_type sign, equivalent_type exp, equivalent_type decimal) : 
            decimal(decimal), exp(exp), sign(sign) {}

        // 函数：设置浮点数实际的指数
        // 参数：
        // - value 为新的指数值
        // 返回：当前对象的的引用
        the_t & real_exp(ixx value){
            the.exp = value + exp_offset;
            return the;
        }

        // 函数：获取浮点数实际的指数[不带检查]
        // 注意：如果该浮点数值为 0，则会得到错误的指数部分
        ixx real_exp_unsafe() const {
            return ixx(the.exp) - exp_offset;
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
            if (not inc::is_same<float_type, f80>){
                return u64(u64(1) << dec_bits | the.decimal);
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
        constexpr uxx decimal_bits(){
            return dec_bits;
        }

        // 函数：包括隐藏位的尾数部分的尾数
        // 注意：
        // - f80 与 f64, f32 的小数部分不同，f80 没有隐藏位
        constexpr uxx decimal_bits_full(){
            return dec_bits + not inc::is_same<float_type, f80>;
        }

        // 函数：该浮点数最大的精度（10进制）
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

xexport_space(mixc::define_mfxx::origin)