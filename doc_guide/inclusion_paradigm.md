# 包含范式
## 作用介绍
情况一：假设在 `b.cpp` 中并没有用到 `a.hpp` 中包含的 `vector`，我们希望能在 `b.cpp` 中隐藏 `std::vector`
```C++
// a.hpp
#pragma once
#include<vector>
...

// b.cpp
#include"a.hpp"
...
```

情况二：提供最小包含，不需要的功能不包含。C++ 认为少就是多，但这不是 `std::string` 孱弱的理由，我们以 `lang/cxx/` 底层字符串库为例，为您提供避免过度设计的指引。
```C++
#pragma once
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
...
// 实例 foo 只会包含 index_of_first 和 cxx 中的基础功能
inc::cxx foo = "hello powerful cat! do you love cat?";
uxx i        = foo.index_of_first("cat");
...

// b.hpp 只包含 lang/cxx/align_center 和 lang/cxx/align_right 功能
#pragma once
#include"lang/cxx/align_center.hpp"
#include"lang/cxx/align_right.hpp"
#include"lang/cxx.hpp"

// c.hpp 包含 lang/cxx/ 中所有功能
#pragma once
#include"lang/cxx/+.hpp"
#include"lang/cxx.hpp"
...
```

情况三：头文件包含了类似 **stdio.h** 这样的库，在不增加 .cpp 文件的情况下怎么隐藏不必要的符号名称
```C++
// a.hpp
#include<stdio.h>
...
```

## 用法
实际的包含范式与以上写法有所出入，下面将我们介绍规范的写法。
```C++
// file:foo/bar.hpp
// 命名规则(推荐)：
// xpack_ ## 目录名_ ## 模块名
#ifndef xpack_foo_bar
#define xpack_foo_bar

// 语言特性包
#include"mixc.hpp"

namespace mixc::foo_bar{ 
    // inline 关键字在现代 C++ 中的语义更偏向于指示该函数主体可以出现在头文件中
    // hidden
    static inline int private_data = 100;

    // hidden
    inline void bar_core(){
        ...
    }

    // visiable
    inline void bar(){
        ...
    }

    // all visiable
    namespace origin{
        enum class bar_t{
            on,
            off,
        };

        constexpr uxx max = 1024;
    }
}

// 这个预处理块只会出现一次
#endif

// 这部分可以出现多次
// 导出单个符号
xexport(mixc::foo_bar::bar)

// 导出命名空间
xexport_space(mixc::foo_bar::origin)

```


```C++
// file:foo/function.hpp
// 命名规则(推荐)：
// xpack_ ## 目录名_ ## 模块名
#ifndef xpack_foo_function
#define xpack_foo_function
#pragma push_macro("xuser")

// 命名规则：
// 项目名::目录名_模块名
// 作用：
// 告诉下面包含的模块，将可见模块导出到 xuser 空间
#define xuser mixc::foo_function::inc
#include"foo/bar.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

// 该命名空间和 xuser 命名保持一致
// 问：为什么不把 #pragma pop_macro("xuser") 放到 #endif 前一行，这样这里就可以用 xexport(mixc::foo_function::inclusion_paradigm)