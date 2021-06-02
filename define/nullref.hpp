/* 模块：nullref
 * 类型：单例·常量·语法糖
 * 功能：通常用于返回引用类型的函数，当函数的输入不是期望值时，可以返回 nullref
 * 用法：
 * type & get(uxx index){
 *     if (index >= this->len){
 *         return nullref;
 *     }
 *     return this->ary[index];
 * }
 * 
 * 注意：请尽量用引用类型接收可能为 nullref 的返回值，必要时需要判断一下返回值是否为 nullref
 * type & ret = get(10);
 * if (ret == nullref){
 *     // may unexpected
 * }
 */


#ifndef xpack_define_nullref_t
#define xpack_define_nullref_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::define_nullref_t::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_nullref_t{
    xstruct(
        xname(nullref_t)
    )
        template<class type_t> bool operator == (type_t const & value) const {
            auto ptr = xref(value);
            return voidp(ptr) == this;
        }

        template<class type_t> friend bool operator == (type_t const & value, nullref_t const & nullref) {
            return nullref.operator==(value);
        }

        template<class type_t> friend bool operator != (type_t const & value, nullref_t const & nullref) {
            return not (nullref == value);
        }

        template<class type_t> friend bool operator != (nullref_t const & nullref, type_t const & value) {
            return not (nullref == value);
        }

        template<class type_t>
        operator type_t & () const {
            return *(type_t *)this;
        }
    $

    inline volatile nullref_t * null    = nullptr;
    inline const    nullref_t & nullref = (nullref_t &) *null;
}

#endif

xexport(mixc::define_nullref_t::nullref)