# 视角
**项目运作观**草案

## 目录
- 如何运行？

## 如何运行？
### 准备
- 将该项目下载到本地
- 编译器 `g++` or `clang`（支持 `std=c++2a` 及其以上版本，g++9 or clang10 或更新的版本）
- 若需调试，还要准备 `gdb`

**如果你用的是 vscode，那么这个问题应该会很好解决：**
- 使用 ctrl+shift+b 快捷键弹出编译提示
- 根据你现有的编译器选择 `g++` 或者 `clang` 编译 `extern.api.o`
- 添加 `main.cpp` 并引用 `Mix-C` 项目的组建书写代码
- 使用 ctrl+shift+b 快捷键弹出编译提示
- 选择 `... build active file` 选项

**其他编辑器：**
- 把 `.vscode/task.json` 中的命令抠下来

**朴素风格的`你好世界`:**
```C++
#define xuser mixc::powerful_cat
#include"io/tty.hpp"

int main(){
    xuser::tty.write_line("hello world");
    return 0;
}

// 输出以下内容：
// hello world
```

**调试风格的`你好世界`:**
```C++
#define xuser mixc::powerful_cat
#include"mixc.hpp"

int main(){
    xhint("hello world");
    return 0;
}

// 输出以下内容：
// HINT | main.cpp:5                                                   | main                 | "hello world"
```
