# COMMENT SPECIFICATION 0.02
**文档注释规范**草案

## 模块描述
### 模块头注释的基本属性
- 模块(名称)
- 类型(呈现形态)
- 功能(描述)
- 用法(用代码描述)
- 注意(可能的误用、陷阱、副作用说明)

### 模块头注释
```C++
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
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::define_nullref_t{
    ...
    ...
```

#### 模块
- 和文件名保持一致
- 名词

#### 类型
```
类型A：
    | 常量       |
    | 只读       |
    | 变量       |

类型B：
    | 基础结构   |   -> 提供一个类的雏形，该类只用于继承和被继承
    | 适配器     |   -> 通过继承该结构来对外提供一致的接口，该类只用于继承和被继承
    | 兼容扩展   |   -> 从基础结构派生的结构，该结构不在原有结构上增加任何字段，只是增加成员函数，并无函数重写，
                        和原有结构保持兼容性，该类只能继承和被继承
    | 非兼容扩展 |   -> 在原有结构上新增了字段，或破坏了和基础结构的兼容性
                        该类只能继承和被继承
    | 接口结构   |   -> 该结构是扩展结构的终点。在头文件中包含某一类的最后一个扩展时需要再包含此结构，
                        该结构整合该类所有的扩展，对外提供所需的功能，只有这个这个结构可以实例化

类型B 关系图：
        +-->[基础结构]           [接口结构]
        |       A                    A
     选 |       | * 直接继承         | * 使用
     一 |       |                    |
     种 +-->[兼容/非兼容扩展]--------+
     直 |       A
     接 |       | * 从模板参数继承
     继 |       |
     承 |-->[适配器]
        |
        +---[接口结构]

类型C：
    | 函数单体   |

```

### 函数注释
```C++
/* 函数：去除重复元素并保证顺序不改变
 * 参数：
 * - seq 为待去重的序列，该类型需要满足 can_unified_seqlize 约束
 * - alloc 为分配回调函数，期望签名如下：
 *      unified_seq<seq_t> operator()(uxx length)
 *   其中 seq_t 和待去重序列的类型保持一致，length 为去重后序列的长度
 * 返回：
 * - 从 alloc 中分配的序列
 */ 
template<
    inc::can_unified_seqlize        seq_t,
    class                           alloc_t
>
requires(
    can_distinct_alloc<seq_t, alloc_t>
)
inline auto distinct(seq_t const & seq, alloc_t const & alloc)
```

#### 属性
- 函数：关于函数作用的简述
- 参数：关于参数列表的描述，标准开头为 "- arg_name 为xxxx" 定义参数的作用
- 返回：描述返回值表示什么
- 注意：根据函数可能的误用或副作用进行说明
