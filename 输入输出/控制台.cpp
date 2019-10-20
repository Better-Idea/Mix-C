#include<Windows.h>
#ifdef _
#undef _
#endif
#include"控制台.h"

结构体 匿名字段{
    HANDLE 输出;
    HANDLE 输入;
};

#define 句柄 (*(::匿名字段 *)$D.匿名字段)

命名空间 控制台空间{
    模板<> 数据<>::数据() {
        句柄.输入 = GetStdHandle(STD_INPUT_HANDLE);
        句柄.输出 = GetStdHandle(STD_OUTPUT_HANDLE);
        色彩.前景 = (U08)灰色;
        色彩.背景 = (U08)黑色;
        SetConsoleTextAttribute(句柄.输出, 色彩);
    }
    模板<> 空 接口<>::前景属性结构::运算重载=(控制台颜色 常量 & 值) 常量 {
        $D.色彩.前景 = (U08)值;
        SetConsoleTextAttribute(句柄.输出, $D.色彩);
    }
    模板<> 空 接口<>::背景属性结构::运算重载=(控制台颜色 常量 & 值) 常量 {
        $D.色彩.背景 = (U08)值;
        SetConsoleTextAttribute(句柄.输出, $D.色彩);
    }
    模板<> 接口<>::前景属性结构::运算重载 控制台颜色() 常量 {
        回递 (控制台颜色)$D.色彩.前景;
    }
    模板<> 接口<>::背景属性结构::运算重载 控制台颜色() 常量 {
        回递 (控制台颜色)$D.色彩.背景;
    }
    模板<> 空 接口<哑元结构>::打印(原始串 串) {
        WriteConsoleW(句柄.输出, 串->指针, (DWORD)串->长度, NULL, NULL);
    }
}

控制台 终端;
