# COMMENT SPECIFICATION 0.02
**文档注释规范**草案

## 模块描述
### 模块头注释的基本属性
- 模块(名称)
- 类型(呈现形态)
- 功能(描述)
- 用法(用代码描述)
- 注意(可能的误用、陷阱说明)

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
#define xuser mixc::define_nullref_t
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
组合：
    | 常量 | x | 语法糖 |
    | 变量 |   | 字面量 |

组合：
    | 单例 | x | 基础结构 |
    | 多例 | x | 扩展函数 |
               | 静态结构 |

其他：
    | 函数单体 |

```

### 函数注释
```C++
/* 函数：去除重复元素
 * 参数：
 * - seq 为满足 inc::unified_seq_t 格式的模板
 * - alloc 为新序列分配回调函数
 * 返回：
 * - 从 alloc 中分配的序列，里面存放着去重后的元素序列
 */ 
 template<inc::unified_seq_t seq_t>
 inline auto distinct(seq_t const & seq, distinct_alloc_invoke<seq_t> alloc);

```

#### 属性
- 函数：关于函数作用的简述
- 参数：关于参数列表的描述，标准开头为 "- arg_name 为xxxx" 定义参数的作用
- 返回：描述返回值表示什么
- 注意：根据函数可能的误用或副作用进行说明
