# 包含范式
## 作用介绍
**假如你不想用 C++20 的 module，我们有挖好的备选方案**  
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

情况二：提供最小包含，不需要的功能不包含。C++ 认为少就是多，但这不是 `std::string` 孱弱的理由，我们以 `lang/cxx/` 底层字符串库为例，为您提供可组装地设计指引。
```C++
#pragma once
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
...
// 实例 foo 只会包含 index_of_first 和 cxx 中的基础功能
cxx foo     = "hello powerful cat! do you love cat?";
uxx i       = foo.index_of_first("cat");
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

情况三：包含了类似 `stdio.h` 这样的头文件，在不增加 `.cpp` 文件的情况下怎么隐藏不必要的符号名称
```C++
// a.hpp
#include<stdio.h>
...
```

## 用法
实际的包含范式与以上写法有所出入，下面我们将介绍规范的写法。
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
// 保存并取消 xuser 定义
#pragma push_macro("xuser")
#undef  xuser

// 命名规则：
// 项目名::目录名_模块名
// 作用：
// 告诉下面包含的模块，将可见模块导出到 mixc::foo_function::inc 空间
#define xuser mixc::foo_function::inc
#include"foo/bar.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

// 该命名空间和 xuser 命名保持一致
// 问：为什么不把 #pragma pop_macro("xuser") 放到 #endif 前一行，
//     这样这里就可以用 namespace xuser{ 这种写法了
// 答：我推荐你使用代码片段生成该包含范式框架。
//     主要时为了避免漏写，如果这个命名空间里包含了较多代码，
//     也许你复制粘贴时会漏了 #pragma pop_macro("xuser")
//     然后你会陷入痛苦的预处理排错阶段
// 问：预编译出错了怎么办？
// 答：使用下方命令（记得更改参数），祝你好运~
//     gcc xxxx.cpp -E -o xxxx.hpp
namespace mixc::foo_function{
    inline void function(){
        // inc 子命名空间只会出现包含的符号
        inc::bar();
    }
}

// 这个预处理块只会出现一次
#endif

// 对外导出 function 函数
xexport(mixc::foo_function::function)

```

## 特殊情况
- 循环包含
- 彻底隐藏

### 循环包含
在 `.h/.cpp` 模式下由于 `.cpp` 可以单独作为一个编译空间，所以 `a.cpp` 可以同时包含 `a.h` 和 `b.h` 而
`b.cpp` 也可以同时包含 `a.h` 和 `b.h`。  
对于只有头文件的情况怎么解决循环包含的情况的呢？方法如下：

#### **file:func/private/a.hpp**
```C++
#ifndef xpack_func_private_a    // 注意这里的命名
#define xpack_func_private_a
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::func_a::inc // 这里不是 mixc::func_private_a
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::func_b::origin{
    struct b;
}

namespace mixc::func_a::inc{
    using namespace mixc::func_b::origin;
}

namespace mixc::func_a::origin{ // 这里不是 mixc::func_private_a
    struct a{
        void invoke(inc::b);    // 这部分放到外部定义

        void other(){           // 对于不存在循环依赖的函数则不做限制
            // do sth inline ...
        }
    };
}

#endif

xexport_space(mixc::func_a::origin)
```

#### **file:func/private/b.hpp**
```C++
#ifndef xpack_func_private_b    // 声明
#define xpack_func_private_b
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::func_b::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::func_a::origin{
    struct a;
}

namespace mixc::func_b::inc{
    using namespace mixc::func_a::origin;
}

namespace mixc::func_b::origin{ // 这里不是 mixc::func_private_a
    struct b{
        void invoke(inc::a);    // 这部分放到外部定义

        void other(){           // 对于不存在循环依赖的函数则不做限制
            // do sth inline ...
        }
    };
}

#endif

xexport_space(mixc::func_b::origin)
```

#### **file:func/a.hpp**
```C++
#ifndef xpack_func_a            // 主体
#define xpack_func_a
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::func_a::inc
#include"func/private/a.hpp"    // private 包放到前面
#include"func/private/b.hpp"
#include"func/b.hpp"            // func/a.hpp 和 func/b.hpp
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::func_a::origin{ // 命名空间与 func/private/a.hpp 保持一致
    // 这里要用 inline 关键字
    // 避免被包含到多个 .cpp 文件后导致符号重定义错误
    inline void a::invoke(inc::b){
        // ...
    }
}

#endif

xexport_space(mixc::func_a::origin)
```

#### **file:func/b.hpp**
```C++
#ifndef xpack_func_b            // 主体 pack
#define xpack_func_b
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::func_b::inc
#include"func/private/a.hpp"    // private 包放到前面
#include"func/private/b.hpp"
#include"func/a.hpp"            // 包含 a
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::func_b::origin{
    // 这里要用 inline 关键字
    // 避免被包含到多个 .cpp 文件后导致符号重定义错误
    inline void b::invoke(inc::a){
        // ...
    }
}

#endif

xexport_space(mixc::func_b::origin)
```

该布局方式让每个头文件只会被包含一次，即使存在循环包含

### 彻底隐藏
对于类似 windows.h 这样的文件，它既庞大又与平台相关。直接包含它会拖慢编译速度同时也会在全局命名空间布满各种符号。
包含范式有两个包含分支，一个是主分支，它结束于 main 函数所在的顶层包含文件。另一个是从分支，该分支就用于隔离 windows.h 这样的头文件，将它预先编译成 .o 文件后重复使用，主分支的变更不不影响该分支。  
经常变动、需要 inline 的文件放到主分支，而稳定的、需要隔离的文件放到从分支。  

**对外**
```C++
// file:foo/bar.hpp
#ifndef xpack_foo_bar           // 主体
#define xpack_foo_bar
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::foo_bar::inc
#include"foo/pack_a.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::foo_bar::origin{
    struct bar{
        void hello();
    };
}

#endif

xexport_space(mixc::foo_bar::origin);
```

**对内**
```C++
// file:foo/private/extern.bar.hpp
// 推荐在该类型的文件名前加 `extern.` 前缀
#ifdef  xuser
#undef  xuser
#endif

// 和 foo/bar.hpp 中保持一致
#define xuser  mixc::foo_bar::inc
#include"foo/bar.hpp"
// 不用重复包含
// #include"foo/pack_a.hpp"

// 肆无忌惮包含 Windows.h、stdio.h 这样的头文件
#include<Windows.h>

// 和 foo/bar.hpp 中保持一致
namespace mixc::foo_bar::origin{
    void bar::hello(){
        // 在这里愉快的调用系统 API
        // ...
    }
}
```

**对内的模块扔到 extern.api.cpp** 这样的源文件中
```C++
// file:extern.api.cpp
#include"foo/private/extern.bar.hpp"
```
