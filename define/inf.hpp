/* 模块：inf/inf_pos/inf_neg
 * 类型：单例·常量·语法糖
 * 功能：提供无穷大、正负无穷大的字面量
 * 用法：
 * f64 fdiv(f64 a, f64 b){
 *     if (b != 0){
 *         return a / b;
 *     }
 *     // 正无穷
 *     if (a > 0){
 *         return inf_pos;
 *     }
 *     // 负无穷
 *     if (a < 0){
 *         return inf_neg;
 *     }
 *     else{
 *         return 1.0;
 *     }
 * }
 * 
 * ...
 * 
 * f64 c = fdiv(a, b);
 * 
 * // c 为正无穷或负无穷
 * if (c == inf){
 *     ...
 * }
 * 
 * f64 d = fdiv(a, b);
 * 
 * // 只要判断正无穷
 * if (d == inf_pos){
 *     ...
 * }
 * 
 * f64 e = fdiv(a, b);
 * 
 * // 只要判断负无穷
 * if (d == inf_neg){
 *     ...
 * }
 * 
 * 注意：三个无穷字面量中，只有 inf 是不能赋值给其他变量的
 */

#ifndef xpack_define_inf
#define xpack_define_inf
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_inf::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"utils/mfxx.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_inf::origin{
    // 正无穷
    constexpr inc::mf64 inf_pos { 0ull, 0x7ffull, 0ull };

    // 负无穷
    constexpr inc::mf64 inf_neg { 1ull, 0x7ffull, 0ull };

    struct inf_t{
        constexpr inf_t(){}

        template<class type_t>
        friend bool operator == (type_t const & value, inf_t){
            return value == inf_pos or value == inf_neg;
        }

        template<class type_t>
        friend bool operator != (type_t const & value, inf_t){
            return value != inf_pos and value != inf_neg;
        }

        template<class type_t>
        friend bool operator == (inf_t, type_t const & value){
            return value == inf_pos or value == inf_neg;
        }

        template<class type_t>
        friend bool operator != (inf_t, type_t const & value){
            return value != inf_pos and value != inf_neg;
        }

        constexpr auto operator +() const {
            return inf_pos;
        }

        constexpr auto operator -() const {
            return inf_neg;
        }
    };

    // 正无穷或负无穷
    constexpr inf_t inf {};
}

#endif

xexport_space(mixc::define_inf::origin)