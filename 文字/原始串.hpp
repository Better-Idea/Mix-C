﻿#pragma once
#include"基本定义/属性范式.hpp"
#define  _                 (原始串) u""

命名空间 原始串空间{
    三体(类 类型);
    模板<类 类型> 结构体 数据{
        卡诺匹斯(类型);
        自然数      长度;
        文字 常量 * 指针;
        符号常量 数据(文字 常量 * 串) {
            指针 = 串;
            只要(串[0]) {
                串++;
            }
            长度 = 串 - 指针;
        }
        数据() {}
        数据(文字 常量 * 指针, 自然数 个数) :
            长度(个数), 指针(指针) {
        }
    };
    
    模板<类 类型> 结构体 接口{
        卡诺匹斯(类型);
        属性(自然数, 长度)
            公有读 { 回递 $D.长度; }
            公有写 { $D.长度 = 值; }
        $
        属性(文字 常量 *, 指针)
            公有读 { 回递 $D.指针; }
            公有写 { $D.指针 = 值; }
        $
    };
    
    模板<类 类型> 结构体 运算{
        卡诺匹斯(类型);
        数据类型 * 运算重载->(){
            回递 (数据类型 *)此指针;
        }
        接口类型 * 运算重载 & () {
            回递 (接口类型 *)此指针;
        }
        运算重载 文字 常量 *(){
            回递 $I.指针;
        }
        文字 运算重载[](自然数 下标) {
            回递 $D.指针[下标];
        }
        整数 运算重载 - (运算 常量 & 值) {
            回递 $D.指针 - ((运算 &)值)->指针;
        }
        类型 运算重载-- (I32) {
            类型 临时 = $D;
            此 -= 1;
            回递 临时;
        }
        类型 运算重载++ (I32) {
            类型 临时 = $D;
            此 += 1;
            回递 临时;
        }
        类型 运算重载-- () {
            此 -= 1;
            回递 此;
        }
        运算类型 运算重载++ () {
            此 += 1;
            回递 此;
        }
        类型 运算重载 - (自然数 偏移) {
            类型 临时 = $D;
            临时 -= 偏移;
            回递 临时;
        }
        类型 运算重载 + (自然数 偏移) {
            类型 临时 = $D;
            临时 += 偏移;
            回递 临时;
        }
        空 运算重载 -= (自然数 偏移) {
            $I.长度 = $I.长度 + 偏移;
            $I.指针 = $I.指针 - 偏移;
        }
        空 运算重载 += (自然数 偏移) {
            $I.长度 = $I.长度 - 偏移;
            $I.指针 = $I.指针 + 偏移;
        }
    };
}

结构体 字符串;
结构体 原始串 : 原始串空间::运算<原始串>{
    符号常量 原始串(文字 常量 * 串):
        数据(串) {
    }
    原始串(){}
    原始串(字符串 &&) = 删;
    原始串(原始串 串, 自然数 长度) :
        数据(串->指针, 长度){
    }
    原始串(原始串空间::数据<原始串> 数据) :
        数据(数据){
    }
    原始串(文字 常量 * 串, 自然数 个数) :
        数据(串, 个数){
    }
    空 运算重载=(字符串 &&) = 删;
私有
    数据类型 数据;
};
