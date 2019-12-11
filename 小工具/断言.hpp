﻿#pragma once
#include<stdio.h>
#include"基本定义/迷你.hpp"
#include"小工具/局部函数.hpp"

符号常量 布尔   已达到错误阈值 = 是;
外部链接 自然数 错误次数;
外部链接 自然数 错误阈值;

内联链接 布尔 条件断言(布尔 条件, U32 行号, 局部函数<空()> 回调 = 局部函数<空()>()) {
    若 (!条件){
        printf("\n  + %-5d 错误     ", 行号);
        回调();
        错误次数++;

        若 (错误次数 == 错误阈值) {
            getchar();
        }
    }
    回递 !条件;
}

模板<类 ... 参数>
内联链接 布尔 条件断言(布尔 条件, I32 行号, 阿斯克码 常量 * 格式, 参数 常量 & ... 列表) {
    回递 条件断言(条件, 行号, $F[&]() {
        printf(格式, 列表...);
    });
}

模板<类 序列类型>
内联链接 空 打印(阿斯克码 常量 * 名称, 序列类型 常量 & 序列, U32 列宽 = 5, 自然数 每行多少个 = 20) {
    char 格式甲[8];
    sprintf(格式甲, "%%-%dd", 列宽);

    循环(自然数 I = 0; I < 序列->长度; I++) {
        若 (I % 每行多少个 == 0) {
            printf("\n %8c %-8s ", ' ', 名称);
        }
        printf(格式甲, 序列[I]);
    }
}

#define 标头(名称)                      \
静态 空 名称() {                        \
    结构体 __RESET__{                   \
        ~__RESET__() {                  \
            若 (错误次数 == 0){         \
                printf(" [通过]");      \
            }                           \
            非{                         \
                错误次数 = 0;           \
            }                           \
        }                               \
    }__R__;                             \
    printf("\n- %s", #名称);

#define 断言(条件,...)  若 (条件断言(条件,__LINE__,##__VA_ARGS__))
#define 展示(名称)      打印(#名称, 名称)

