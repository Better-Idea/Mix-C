﻿#pragma once
#include"基本定义/属性范式.hpp"
#include"小工具/绑定位.hpp"

命名空间 静态位向量空间{
    三体(自然数 位数, 类 二值 = 布尔);
    模板<自然数 位数, 类 二值>
    结构体 数据{
        卡诺匹斯(位数, 二值);
        U08 元素[位数 / 8 + (位数 % 8 != 0)];
        数据() : 元素{ 0 } {}

        模板<类 ... 参数>
        数据(参数 常量 & ... 列表) : 数据() {
            隐式值<自然数> 序列[] = { (参数 & ...)列表... };
            循环(自然数 I = 0; I < 参数数(列表); I++) {
                $O[序列[I]] = 二值(1);
            }
        }
    };

    模板<自然数 位数, 类 二值>
    结构体 接口{
        卡诺匹斯(位数, 二值);
        静态 符号常量 自然数 长度 = 位数;
        空 清空(){
            再初始化(数据类型, $D)();
        }
    };

    模板<自然数 位数, 类 二值>
    结构体 运算{
        运算型卡诺匹斯(位数, 二值);
        绑定位<U08, 二值> 运算重载[](自然数 下标) {
            回递 绑定位<U08, 二值>($D.元素[下标 >> 3], 下标 & 0x7);
        }
    };
}

模板<自然数 位数, 类 二值 = 布尔>
结构体 静态位向量 : 静态位向量空间::运算<位数, 二值>{
    静态位向量() {}
    模板<类 ... 参数>
    静态位向量(参数 常量 & ... 列表): 
        数据(列表...){
    }
私有
    静态位向量空间::数据<位数, 二值> 数据;
};
