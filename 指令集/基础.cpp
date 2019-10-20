﻿#include"基础.h"
#ifdef _MSC_VER
#include<intrin.h>

命名空间 指令集 {
    #if 六十四位系统
    U64 加法(U64 & 低64位, U64 甲, U64 乙) {
        回递 (U64)_addcarry_u64(0, 甲, 乙, & 低64位);
    }
    U64 加法(U64 & 低64位, U64 甲, U64 乙, U64 进位) {
        回递 (U64)_addcarry_u64(U08(进位), 甲, 乙, &低64位);
    }
    U64 减法(U64 & 低64位, U64 甲, U64 乙) {
        回递 (U64)_subborrow_u64(0, 甲, 乙, & 低64位);
    }
    U64 减法(U64 & 低64位, U64 甲, U64 乙, U64 借位) {
        回递 (U64)_subborrow_u64(U08(借位), 甲, 乙, &低64位);
    }
    U64 乘法(U64 & 低64位, U64 甲, U64 乙) {
        U64 高64位;
        低64位 = _mulx_u64(甲, 乙, & 高64位);
        回递 高64位;
    }
    U64 除法(U64 & 低64位, U64 甲, U64 乙) {
        低64位 = 甲 / 乙;
        回递 甲 % 乙;
    }
    I64 乘法(I64 & 低64位, I64 甲, I64 乙) {
        I64 高64位;
        低64位 = _mul128(甲, 乙, & 高64位);
        回递 高64位;
    }
    I64 除法(I64 & 低64位, I64 甲, I64 乙) {
        低64位 = 甲 / 乙;
        回递 甲 % 乙;
    }
    布尔 位测试(U64 值, 自然数 下标) {
        回递 _bittest64((__int64 *)& 值, __int64(下标)) != 0;
    }
    布尔 位测试后位取反(U64 & 值, 自然数 下标) {
        回递 _bittestandcomplement64((__int64  *)& 值, __int64(下标)) != 0;
    }
    布尔 位测试后位置位(U64 & 值, 自然数 下标) {
        回递 _bittestandset64((__int64 *)& 值, __int64(下标)) != 0;
    }
    布尔 位测试后位复位(U64 & 值, 自然数 下标) {
        回递 _bittestandreset64((__int64 *)& 值, __int64(下标)) != 0;
    }
    自然数 置位位个数(U64 值) {
        回递 ((U64)__popcnt64(值));
    }
    自然数 复位位个数(U64 值) {
        回递 置位位个数(~值);
    }
    自然数 低位零个数(U64 值) {
        回递 _tzcnt_u64(值);
    }
    自然数 高位零个数(U64 值) {
        回递 _lzcnt_u64(值);
    }
    自然数 低位一个数(U64 值) {
        回递 低位零个数(~值);
    }
    自然数 高位一个数(U64 值) {
        回递 高位零个数(~值);
    }
    自然数 最低置位位索引(U64 值){
        unsigned long R;
        若 (_BitScanForward64(&R, 值) == 否){
            回递 不存在;
        }
        回递 (U64)R;
    }
    自然数 最高置位位索引(U64 值){
        unsigned long R;
        若 (_BitScanReverse64(&R, 值) == 否){
            回递 不存在;
        }
        回递 (U64)R;
    }
    #elif 三十二位系统
    U64 加法(U64 & 低64位, U64 甲, U64 乙) {
        回递 加法(低64位, 甲, 乙, 0);
    }
    U64 加法(U64 & 低64位, U64 甲, U64 乙, U64 进位) {
        字化对象<U64, U32> C = 低64位;
        字化对象<U64, U32> A = 甲;
        字化对象<U64, U32> B = 乙;
        U08 CF = (U08)进位;
        CF = _addcarry_u32(CF, A[0], B[0], & C[0]);
        CF = _addcarry_u32(CF, A[1], B[1], & C[1]);
        回递 (U64)CF;
    }
    U64 减法(U64 & 低64位, U64 甲, U64 乙) {
        回递 减法(低64位, 甲, 乙, 1);
    }
    U64 减法(U64 & 低64位, U64 甲, U64 乙, U64 借位) {
        字化对象<U64, U32> C = 低64位;
        字化对象<U64, U32> A = 甲;
        字化对象<U64, U32> B = 乙;
        U08 CF = (U08)借位;
        CF = _subborrow_u32(CF, A[0], B[0], & C[0]);
        CF = _subborrow_u32(CF, A[1], B[1], & C[1]);
        回递 (U64)CF;
    }
    U64 乘法(U64 & 低64位, U64 甲, U64 乙) {
        U64 高64位;
        字化对象<U64, U32> Ta = 甲;
        字化对象<U64, U32> Tb = 乙;
        U32 A = Ta[0];
        U32 B = Ta[1];
        U32 C = Tb[0];
        U32 D = Tb[1];
        U32 L[4];
        U32 H[4];
        U08 CF = 0;

        H[0] = _mulx_u32(A, C, & L[0]);
        H[1] = _mulx_u32(A, D, & L[1]);
        H[2] = _mulx_u32(B, C, & L[2]);
        H[3] = _mulx_u32(B, D, & L[3]);

        CF = _addcarry_u32(CF, H[0], L[1], L + 1);
        CF = _addcarry_u32(CF, L[1], L[2], L + 1);
        CF = _addcarry_u32(CF, H[1], H[2], H + 2);
        CF = _addcarry_u32(CF, H[2], L[3], H + 2);
        H[3] += CF;

        低64位 = (U64)L[1] << 32 | L[0];
        高64位 = (U64)H[1] << 32 | H[0];
        回递 高64位;
    }
    U64 除法(U64 & 低64位, U64 甲, U64 乙) {
        低64位 = 甲 / 乙;
        回递 甲 % 乙;
    }
    I64 乘法(I64 & 低64位, I64 甲, I64 乙) {
        U64 L;
        U64 H;
        U64 CF = 0;
        I64 高64位;
        H = 乘法(L, U64(甲), U64(乙));

        若 (位测试(U64(甲), 63)) {
            若 (位测试(U64(乙), 63)) { //负负得正
                CF = 减法(L, CF, L);
                CF = 减法(H, CF, H);
            }
            非{
                H |= U64(-1) << 最高置位位索引(H);
            }
        }
        非{
            若 (位测试(U64(乙), 63)) {
                H |= U64(-1) << 最高置位位索引(H);
            }
        }

        低64位 = (I64)L;
        高64位 = (I64)H;
        回递 高64位;
    }
    I64 除法(I64 & 低64位, I64 甲, I64 乙) {
        低64位 = 甲 / 乙;
        回递 甲 % 乙;
    }
    布尔 位测试(U64 值, 自然数 下标) {
        字化对象<U64, U32> T = 值;
        回递 下标 >= 32 ? 
            位测试(T[1], 下标 - 32) : 
            位测试(T[0], 下标);
    }
    布尔 位测试后位取反(U64 & 值, 自然数 下标) {
        字化对象<U64, U32> T = 值;
        回递 下标 >= 32 ? 
            位测试后位取反(T[1], 下标 - 32) : 
            位测试后位取反(T[0], 下标);
    }
    布尔 位测试后位置位(U64 & 值, 自然数 下标) {
        字化对象<U64, U32> T = 值;
        回递 下标 >= 32 ?
            位测试后位置位(T[1], 下标 - 32) :
            位测试后位置位(T[0], 下标);
    }
    布尔 位测试后位复位(U64 & 值, 自然数 下标) {
        字化对象<U64, U32> T = 值;
        回递 下标 >= 32 ?
            位测试后位复位(T[1], 下标 - 32) :
            位测试后位复位(T[0], 下标);
    }
    自然数 置位位个数(U64 值) {
        字化对象<U64, U32> T = 值;
        回递 置位位个数(T[0]) + 置位位个数(T[1]);
    }
    自然数 复位位个数(U64 值) {
        回递 置位位个数(~值);
    }
    自然数 低位零个数(U64 值) {
        字化对象<U64, U32> T = 值;
        回递 
            低位零个数(T[0]) & 0x20 ?
            低位零个数(T[0]) : 
            低位零个数(T[0]) + 低位零个数(T[1]);
    }
    自然数 高位零个数(U64 值) {
        字化对象<U64, U32> T = 值;
        回递 
            高位零个数(T[1]) & 0x20 ?
            高位零个数(T[1]) + 0x20 : 
            高位零个数(T[0]);
    }
    自然数 最低置位位索引(U64 值){
        字化对象<U64, U32> T = 值;
        unsigned long R;

        若 (_BitScanForward(& R, T[0])){
            若 (_BitScanForward(& R, T[1])) {
                回递 不存在;
            }
            回递 自然数(R) + 32;
        }
        回递 自然数(R);
    }
    自然数 最高置位位索引(U64 值){
        字化对象<U64, U32> T = 值;
        unsigned long R;

        若 (_BitScanForward(& R, T[1])){
            若 (_BitScanForward(& R, T[0])) {
                回递 不存在;
            }
            回递 自然数(R);
        }
        回递 自然数(R) + 32;
    }
    #endif

    布尔 位测试(U32 值, 自然数 下标) {
        回递 _bittest((long *)& 值, long(下标)) != 0;
    }
    布尔 位测试(U16 值, 自然数 下标) {
        U32 临时 = 值;
        回递 _bittest((long *)& 临时, long(下标)) != 0;
    }
    布尔 位测试(U08 值, 自然数 下标) {
        U32 临时 = 值;
        回递 _bittest((long*)& 临时, long(下标)) != 0;
    }
    布尔 位测试后位置位(U32 & 值, 自然数 下标) {
        回递 _bittestandset((long *)& 值, long(下标)) != 0;
    }
    布尔 位测试后位置位(U16 & 值, 自然数 下标) {
        U32  临时 = 值;
        布尔 结果 = _bittestandset((long*)& 临时, long(下标)) != 0;
        值 = (U16)临时;
        回递 结果;
    }
    布尔 位测试后位置位(U08 & 值, 自然数 下标) {
        U32  临时 = 值;
        布尔 结果 = _bittestandset((long*)& 临时, long(下标)) != 0;
        值 = (U08)临时;
        回递 结果;
    }
    布尔 位测试后位复位(U32 & 值, 自然数 下标) {
        回递 _bittestandreset((long *)& 值, long(下标)) != 0;
    }
    布尔 位测试后位复位(U16 & 值, 自然数 下标) {
        U32  临时 = 值;
        布尔 结果 = _bittestandreset((long*)& 临时, long(下标)) != 0;
        值 = (U16)临时;
        回递 结果;
    }
    布尔 位测试后位复位(U08 & 值, 自然数 下标) {
        U32  临时 = 值;
        布尔 结果 = _bittestandreset((long*)& 临时, long(下标)) != 0;
        值 = (U08)临时;
        回递 结果;
    }
    布尔 位测试后位取反(U32 & 值, 自然数 下标) {
        回递 _bittestandcomplement((long *)& 值, long(下标)) != 0;
    }
    布尔 位测试后位取反(U16 & 值, 自然数 下标) {
        U32  临时 = 值;
        布尔 结果 = _bittestandcomplement((long*)& 临时, long(下标)) != 0;
        值 = (U16)临时;
        回递 结果;
    }
    布尔 位测试后位取反(U08 & 值, 自然数 下标) {
        U32  临时 = 值;
        布尔 结果 = _bittestandcomplement((long*)& 临时, long(下标)) != 0;
        值 = (U08)临时;
        回递 结果;
    }

    自然数 置位位个数(U32 值) {
        回递 (自然数)__popcnt(值);
    }
    自然数 置位位个数(U16 值) {
        回递 (自然数)__popcnt((U32)值);
    }
    自然数 置位位个数(U08 值) {
        回递 (自然数)__popcnt((U32)值);
    }
    自然数 复位位个数(U32 值) {
        回递 (自然数)__popcnt(~值);
    }
    自然数 复位位个数(U16 值) {
        回递 (自然数)__popcnt(~(U32)值);
    }
    自然数 复位位个数(U08 值) {
        回递 (自然数)__popcnt(~(U32)值);
    }
    自然数 低位零个数(U32 值) {
        回递 _tzcnt_u32(值);
    }
    自然数 低位零个数(U16 值) {
        回递 _tzcnt_u32(值);
    }
    自然数 低位零个数(U08 值) {
        回递 _tzcnt_u32(值);
    }
    自然数 高位零个数(U32 值) {
        回递 _lzcnt_u32(值);
    }
    自然数 高位零个数(U16 值) {
        回递 _lzcnt_u32(值);
    }
    自然数 高位零个数(U08 值) {
        回递 _lzcnt_u32(值);
    }
    
    自然数 最低置位位索引(U32 值) {
        unsigned long R;
        若 (_BitScanForward(&R, (unsigned long)值) == 否) {
            回递 不存在;
        }
        回递 (自然数)R;
    }
    自然数 最低置位位索引(U16 值) {
        回递 最低置位位索引((U32)值);
    }
    自然数 最低置位位索引(U08 值) {
        回递 最低置位位索引((U32)值);
    }
    自然数 最高置位位索引(U32 值) {
        unsigned long R;
        若 (_BitScanReverse(&R, (unsigned long)值) == 否) {
            回递 不存在;
        }
        回递 (自然数)R;
    }
    自然数 最高置位位索引(U16 值) {
        回递 最高置位位索引((U32)值);
    }
    自然数 最高置位位索引(U08 值) {
        回递 最高置位位索引((U32)值);
    }

    自然数 最低复位位索引(U64 值) { 
        回递 最低置位位索引((U64)~值); 
    }
    自然数 最低复位位索引(U32 值) { 
        回递 最低置位位索引((U32)~值); 
    }
    自然数 最低复位位索引(U16 值) { 
        回递 最低置位位索引((U16)~值); 
    }
    自然数 最低复位位索引(U08 值) { 
        回递 最低置位位索引((U08)~值); 
    }
    自然数 最高复位位索引(U64 值) { 
        回递 最高置位位索引((U64)~值); 
    }
    自然数 最高复位位索引(U32 值) { 
        回递 最高置位位索引((U32)~值); 
    }
    自然数 最高复位位索引(U16 值) { 
        回递 最高置位位索引((U16)~值); 
    }
    自然数 最高复位位索引(U08 值) { 
        回递 最高置位位索引((U08)~值); 
    }
    U08 循环左移(U08 被移数, 自然数 移数) {
        回递 _rotl8(被移数, int(移数));
    }
    U16 循环左移(U16 被移数, 自然数 移数) {
        回递 _rotl16(被移数, int(移数));
    }
    U32 循环左移(U32 被移数, 自然数 移数) {
        回递 _rotl(被移数, int(移数));
    }
    U64 循环左移(U64 被移数, 自然数 移数) {
        回递 _rotl64(被移数, int(移数));
    }
    U08 循环右移(U08 被移数, 自然数 移数) {
        回递 _rotr8(被移数, int(移数));
    }
    U16 循环右移(U16 被移数, 自然数 移数) {
        回递 _rotr16(被移数, int(移数));
    }
    U32 循环右移(U32 被移数, 自然数 移数) {
        回递 _rotr(被移数, int(移数));
    }
    U64 循环右移(U64 被移数, 自然数 移数) {
        回递 _rotr64(被移数, int(移数));
    }
    U64 时间戳() {
        回递 __rdtsc();
    }
}
#else

命名空间 指令集 {
    #ifdef 六十四位系统
    U64 加法(U64 & 低64位, U64 甲, U64 乙){
        联合体{
            U64 结果;
            U08 读取;
        }高64位 = { 0 };

        asm("add %2, %%rax":"=a"(低64位):"a"(甲),"r"(乙));
        asm("setcb %0":"=r"(高64位.读取));
        回递 高64位.结果;
    }
    U64 加法(U64 & 低64位, U64 甲, U64 乙, U64 进位){
        联合体{
            U64 结果;
            U08 读取;
        }高64位 = { 0 };
        asm("bt %0, %1"::"i"(0),"r"(进位));
        asm("adc %2, %%rax":"=a"(低64位):"a"(甲),"r"(乙));
        asm("setcb %0":"=r"(高64位.读取));
        回递 高64位.结果;
    }
    U64 减法(U64 & 低64位, U64 甲, U64 乙){
        联合体{
            U64 结果;
            U08 读取;
        }高64位 = { 0 };
        低64位 = 甲;
        asm("sub %2, %%rax":"=a"(低64位):"a"(甲),"r"(乙));
        asm("setcb %0":"=r"(高64位.读取));
        回递 高64位.结果;
    }
    U64 减法(U64 & 低64位, U64 甲, U64 乙, U64 借位){
        联合体{
            U64 结果;
            U08 读取;
        }高64位 = { 0 };
        低64位 = 甲;
        asm("bt %0, %1"::"i"(0),"r"(借位));
        asm("sbb %2, %%rax":"=a"(低64位):"a"(甲),"r"(乙));
        asm("setcb %0":"=r"(高64位.读取));
        回递 高64位.结果;
    }
    U64 乘法(U64 & 低64位, U64 甲, U64 乙){
        U64 高64位;
        asm("mulq %3":"=a"(低64位),"=d"(高64位):"a"(甲),"g"(乙));
        回递 高64位;
    }
    U64 除法(U64 & 低64位, U64 甲, U64 乙){
        U64 高64位;
        asm("divq %3":"=a"(低64位),"=d"(高64位):"a"(甲),"g"(乙));
        回递 高64位;
    }
    I64 乘法(I64 & 低64位, I64 甲, I64 乙){
        U64 高64位;
        asm("imulq %3":"=a"(低64位),"=d"(高64位):"a"(甲),"g"(乙));
        回递 高64位;
    }
    I64 除法(I64 & 低64位, I64 甲, I64 乙){
        U64 高64位;
        asm("idivq %3":"=a"(低64位),"=d"(高64位):"a"(甲),"g"(乙));
        回递 高64位;
    }
    自然数 置位位个数(U64 值) {
        asm("popcntq %0, %0":"=r"(值));
        回递 值;
    }
    #endif

    U32 置位位个数(U32 值) {
        asm("popcntl %0, %0":"=r"(值));
        回递 值;
    }
}
#endif

#include<atomic>

命名空间 指令集 {
    #define 原子操作(类型)                                                  \
    空 原子自增一(类型 & 甲) {                                              \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        临时++;                                                             \
    }                                                                       \
    空 原子自减一(类型 & 甲) {                                              \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        临时--;                                                             \
    }                                                                       \
    空 原子加法(类型 & 甲, 类型 乙) {                                       \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        临时 += 乙;                                                         \
    }                                                                       \
    空 原子减法(类型 & 甲, 类型 乙) {                                       \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        临时 -= 乙;                                                         \
    }                                                                       \
    类型 原子交换(类型 & 甲, 类型 & 乙) {                                   \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        回递(临时.exchange(乙));                                            \
    }                                                                       \
    类型 原子与(类型 & 甲, 类型 乙) {                                       \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        回递(临时 &= 乙);                                                   \
    }                                                                       \
    类型 原子或(类型 & 甲, 类型 乙) {                                       \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        回递(临时 |= 乙);                                                   \
    }                                                                       \
    类型 原子异或(类型 & 甲, 类型 乙) {                                     \
        引用空间 std;                                                       \
        atomic<类型 > & 临时 = *(atomic<类型 > *)(&甲);                     \
        回递(临时 ^= 乙);                                                   \
    }                                                                       

    原子操作(U08);
    原子操作(U16);
    原子操作(U32);
    原子操作(U64);

    原子操作(I08);
    原子操作(I16);
    原子操作(I32);
    原子操作(I64);
}