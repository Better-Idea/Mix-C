﻿#include"小工具/传递优化.h"

定义 U32            数据;
定义 传递优化<数据> 包裹;

结构体 甲{
    包裹 派送(U32 信) {
        回递 构造<U32>(信);
    }
};

主函数() {
    自然数 指示器;
    {
        包裹   代收点;
        包裹   顾客;
        甲   * 顾客的手机;
        甲  ** 某某的手机;
        {                                 //他的小生活圈
            静态 甲        某某;
            顾客的手机 = & 某某;
            某某的手机 = & 顾客的手机;
            指示器 = 分配平衡指示器();    //0
            某某.派送(1024);              //包裹没人要？是不是他发的方式不对？
            指示器 = 分配平衡指示器();    //0
            某某.派送(520) 转交给 代收点; //锲而不舍
            指示器 = 分配平衡指示器();    //1
                                          //她迟迟未来
        }
        指示器 = 分配平衡指示器();        //1
        某某的手机[0] = 空值;             //他死心了
        代收点 转交给 顾客;               //她心里有一点后悔
    }
    指示器 = 分配平衡指示器();            //0 也许这样对彼此都好一点
    回递 0;
}