#pragma once
#include"基本定义/迷你.hpp"
#include<math.h>

命名空间 数学{
    F64 Pow(F64 底数, F64 指数) {
        回递 pow(底数, 指数);
    }
    F64 Mod(F64 被模数, F64 模数) {
        回递 fmod(被模数, 模数);
    }
    F64 Sin(F64 值) {
        回递 sin(值);
    }
    F64 Cos(F64 值) {
        回递 cos(值);
    }
    F64 Tan(F64 值) {
        回递 tan(值);
    }
    F64 Asin(F64 值) {
        回递 asin(值);
    }
    F64 Acos(F64 值) {
        回递 acos(值);
    }
    F64 Atan(F64 值) {
        回递 atan(值);
    }
    F64 Ln(F64 值) {
        回递 log(值);
    }
    F64 Log2(F64 值) {
        回递 log2(值);
    }
    F64 Log10(F64 值) {
        回递 log10(值);
    }
    F64 Log(F64 底数, F64 真数) {
        回递 log2(真数) / log2(底数);
    }

    模板<类 类型> 类型 Max(类型 A, 类型 B) {
        回递 A > B ? A : B;
    }
    模板<类 类型> 类型 Min(类型 A, 类型 B) {
        回递 A < B ? A : B;
    }
    模板<类 类型> 类型 Abs(类型 A) {
        回递 A < 0 ? -A : A;
    }
    符号常量 F64 Pi = 3.14159265358979323846;
    符号常量 F64 E  = 2.71828182845904523536;
}
