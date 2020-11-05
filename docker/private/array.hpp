/* 模块：array
 * 类型：基础结构
 * 功能：提供静态的一维或多维数组
 * 用法：
 * TODO======================================================================================
 */

#ifndef xpack_docker_array
#define xpack_docker_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_array::inc
#include"docker/private/adapter.array_access.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_array{
    /* 静态数组类型 */
    template<class type, uxx count>
    using items_t = type[count];

    /* 结构：静态数组
     * 参数：
     * - final 为接口结构类型
     * - type 为第一维度元素类型
     * - count 为第一维度元素个数
     * - rest 为更高维度元素个数
     */
    template<class final, class type, uxx count = 0, uxx ... rest>
    xstruct(
        xtmpl(array_t, final, type, count, rest...),
        xprif(data, mutable items_t<typename array_t<final, type, rest...>::the_t, count>)
    )
        using item_t = typename array_t<final, type, rest...>::the_t;
    public:
        constexpr array_t() : data(){}
        constexpr array_t(array_t const &) = default;

        template<class ... args>
        constexpr array_t(item_t const & first, args const & ... list) : 
            data { first, ((item_t)list)... } {}

        template<class finalx>
        constexpr array_t(array_t<finalx, type, count, rest...> const & self) : 
            array_t((the_t &)(array_t<finalx, type, count, rest...> &)self) {
        }

        /* 函数：下标随机访问
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t & operator[] (uxx index) {
            return data[index];
        }

        /* 函数：下标随机访问（const 修饰）
         * 参数：
         * - index 要访问元素的下标
         * 返回：
         * - 指定索引的元素的引用
         */
        item_t const & operator[] (uxx index) const {
            return data[index];
        }

        /* 函数：获取数组元素的首地址 */
        operator item_t *() {
            return data;
        }

        /* 函数：获取数组元素的首地址（const 修饰） */
        operator item_t const *() const {
            return data;
        }

        /* 属性：当前维度数组元素的个数 */
        xpubgetx(length, uxx){
            return count;
        }
    $

    template<class final, class type>
    struct array_t<final, type>{
        using the_t = type;
    };

    template<class final, class type, uxx count, uxx ... rest>
    using array = inc::adapter_array_access<
        array_t<final, type, count, rest...>
    >;
}

#endif

#include"math/index_system.hpp"

#undef  xusing_docker_array
#define xusing_docker_array     ::mixc::docker_array
