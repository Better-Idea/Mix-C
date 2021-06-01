/* 模块：nan
 * 类型：单例·常量·语法糖
 * 功能：提供用于比较、赋值的浮点非数 NaN 的标准形式
 * 用法：
 * 
 * // handling
 * if (float_item == nan){
 *     ...
 * }
 * 
 * // assign standard NaN
 * float_item = nan;
 */

#ifndef xpack_define_nan
#define xpack_define_nan
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_nan::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_nan{
    struct nan_t{
        constexpr nan_t(){}

        #define xgen(type)                                      \
        friend inline auto operator == (type value, nan_t) {    \
            return !(value == value);                           \
        }                                                       \
                                                                \
        friend inline auto operator == (nan_t, type value) {    \
            return !(value == value);                           \
        }                                                       \
                                                                \
        friend inline auto operator != (type value, nan_t) {    \
            return (value == value);                            \
        }                                                       \
                                                                \
        friend inline auto operator != (nan_t, type value) {    \
            return (value == value);                            \
        }

        xgen(f32)
        xgen(f64)
        xgen(f80)
        #undef  xgen

        operator f32 () const{
            auto v = u32(-1) >> 1;
            return *(f32 *) & v;
        }

        operator f64 () const {
            auto v = u64(-1) >> 1;
            return *(f64 *) & v;
        }
    };

    constexpr nan_t nan {};
}
#endif

xexport(mixc::define_nan::nan)