﻿#pragma once
#include"基本定义/迷你.hpp"
#include"基本定义/模板元.hpp"
#include"小工具/局部函数.hpp"

命名空间 二分查找空间{
    结构体 查找结果{
        自然数 刚好匹配;
        自然数 小于目标;
        自然数 大于目标;

        查找结果() {
            刚好匹配 = 不存在;
            小于目标 = 不存在;
            大于目标 = 不存在;
        }
    };

    模板<类 序列类型, 类 元素类型>
    查找结果 查找(
        序列类型     常量 & 序列,
        自然数              起始下标,
        自然数              长度,
        元素类型     常量 & 比对值,
        比较回调<元素类型>  回调 = 默认比较<元素类型>()) {

        若 (起始下标 >= 长度) {
            回递 查找结果();
        }
        查找结果   结果;
        序列类型 & 列表 = (序列类型 &)序列;
        自然数     左 = 起始下标;
        自然数     中 = 长度 >> 1;
        自然数     右 = 长度 - 1;
        整数       比较结果;

        只要(左 + 1 <= 右) {
            比较结果 = 回调(列表[中], 比对值);
            若 (比较结果 > 0) {
               右 = 中;
               中 = 自然数((U64(右) + U64(左)) >> 1);
            }
            或 (比较结果 < 0) {
               左 = 中;
               中 = 自然数((U64(右) + U64(左) + 1) >> 1);
            }
            非 {
                结果.刚好匹配 = 中;
                回递 结果;
            }
        }

        比较结果 = 回调(列表[左], 比对值);

        若 (比较结果 == 0) {
            结果.刚好匹配 = 左;
        }
        或 (比较结果 < 0){
            比较结果 = 回调(列表[右], 比对值);
            若 (比较结果 == 0){
                结果.刚好匹配 = 右;
            }
            或 (比较结果 < 0){
                结果.小于目标 = 左;
            }
            非{
                结果.小于目标 = 左;
                结果.大于目标 = 右;
            }
        }
        非{
            结果.大于目标 = 左;
        }
        回递 结果;
    }

    模板<类 序列类型>
    结构体 被查对象{
    私有
        序列类型     & 对象;
        定义 纯元素<解析类型(对象[0])> 元素类型;
        定义 比较回调<元素类型>        比较方法;
        自然数         长度;
        元素类型       比对值;
        结构体{
            自然数     起始下标;
        }附加;

        查找结果 查找(比较方法 常量 & 回调) {
            回递 二分查找空间::查找<序列类型, 元素类型>(对象, 附加.起始下标, 长度, 比对值, 回调);
        }
    公有
        模板<类 序列类型类型>
        被查对象(序列类型类型 常量 & 对象, 元素类型 常量 & 比对值) :
            被查对象(对象, 对象->长度, 比对值){
        }
        
        被查对象(序列类型 常量 & 对象, 自然数 长度, 元素类型 常量 & 比对值) :
            对象((序列类型 &)对象), 长度(长度), 比对值((元素类型 &)比对值){
            附加.起始下标 = 0; 
        }

        推导类型 & 起始下标(自然数 值) {
            附加.起始下标 = 值;
            回递 此;
        }

        推导类型 运算重载->() {
            回递 此指针;
        }

        自然数 刚好匹配(比较方法 回调 = 默认比较<元素类型>()) {
            回递 查找(回调).刚好匹配;
        }

        自然数 向上匹配(比较方法 回调 = 默认比较<元素类型>()) {
            推导类型 结果 = 查找(回调);
            回递 结果.刚好匹配 == 不存在 ? 结果.小于目标 : 结果.刚好匹配;
        }

        自然数 向下匹配(比较方法 回调 = 默认比较<元素类型>()) {
            推导类型 结果 = 查找(回调);
            回递 结果.刚好匹配 == 不存在 ? 结果.大于目标 : 结果.刚好匹配;
        }
    };
}

模板<类 序列类型, 类 比对类型>
二分查找空间::被查对象<序列类型> 二分查找(序列类型 常量 & 对象, 比对类型 常量 & 比对值){
    回递 二分查找空间::被查对象<序列类型>(对象, (比对类型 &)比对值);
};

模板<类 序列类型, 类 比对类型>
二分查找空间::被查对象<序列类型> 二分查找(序列类型 常量 & 对象, 自然数 长度, 比对类型 常量 & 比对值){
    回递 二分查找空间::被查对象<序列类型>(对象, 长度, (比对类型 &)比对值);
};

