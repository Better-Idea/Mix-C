#pragma once
#include"基本定义/迷你.hpp"

模板<类 签名> 结构体 局部函数;

命名空间 局部函数空间{
    模板<类 函数> 联合体 移除对象标记;
    模板<类 返回值, 类 ... 参数>
    联合体 移除对象标记<返回值(参数...)> {
        定义 返回值(结果)(参数...);
    };
    模板<类 返回值, 类 对象, 类 ... 参数>
    联合体 移除对象标记<返回值(对象::*)(参数...) 常量> {
        定义 返回值(结果)(参数...);
    };

    结构体 匿名函数{
        模板<类 函数对象>
        推导类型 运算重载 * (函数对象 常量 & 临时) 常量 {
            定义 解析类型(& 函数对象::运算重载()) 函数类型;
            定义 类型名称 移除对象标记<函数类型>::结果 签名;
            回递 局部函数<签名>(临时);
        }
    };

    符号常量 匿名函数 语法糖{};
}

模板<类 返回值, 类 ... 参数> 
结构体 局部函数<返回值(参数...)>{
私有
    结构体 哑元类{
        返回值 空闭包(参数...) {
            回递 返回值();
        }
    };
    定义 返回值(哑元类::* 闭包类型)(参数...);
    哑元类   * 对象;
    闭包类型   闭包;
公有
    局部函数(){
        对象 = 空值;
        闭包 = & 哑元类::空闭包;
    }

    模板<类 函数对象> 
    显式 局部函数(函数对象 常量 & 值) {
        此 = 值;
    }
    
    模板<类 函数对象> 
    空 运算重载 = (函数对象 常量 & 值){
        设置闭包(& 函数对象::运算重载());
        复制(对象, & 值);
    }
    
    返回值 运算重载()(参数 ... 参数列表){
        回递 (对象->*闭包)(参数列表...);
    }

    运算重载 布尔() {
        回递 闭包 != 空值;
    }
私有
    模板<类 函数对象>
    空 设置闭包(返回值 (函数对象::* 匿名函数)(参数...)){
        复制(闭包, 匿名函数);
    }

    模板<类 函数对象>
    空 设置闭包(返回值 (函数对象::* 匿名函数)(参数...) 常量){
        复制(闭包, 匿名函数);
    }
};

#define $F      局部函数空间::语法糖 * 

