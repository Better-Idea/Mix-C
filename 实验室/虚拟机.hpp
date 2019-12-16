#pragma once
#include"基本定义/迷你.hpp"
#include"指令集/基础.hpp"

枚举 指令区间 : U08{
    PSI  = 0                  , PSI指令条数  = 16, // 立即数装载
    MRW  = PSI  + PSI指令条数 , MRW指令条数  = 16, // 内存读写
    SUB  = MRW  + MRW指令条数 , SUB指令条数  = 16, // 加法
    SBB  = SUB  + SUB指令条数 / 2                , // 加法
    DIV  = SUB  + SUB指令条数 , DIV指令条数  = 16, // 除法
    DIVI = DIV  + DIV指令条数 / 2                , // 除法
    SHL  = DIV  + DIV指令条数 , SHL指令条数  = 16, // 左移
    SHLC = SHL  + SHL指令条数 / 2                , // 左移
    SHR  = SHL  + SHL指令条数 , SHR指令条数  =  8, // 右移
    SAR  = SHR  + SHR指令条数 , SAR指令条数  =  8, // 算数右移
    ADD  = SAR  + SAR指令条数 , ADD指令条数  =  8, // 加法
    ADC  = ADD  + ADD指令条数                    , // 加法
    MUL  = ADD  + ADD指令条数 , MUL指令条数  =  8, // 乘法
    MULI = MUL  + MUL指令条数 / 2                , // 乘法
    AND  = MUL  + MUL指令条数 , AND指令条数  =  4, // 位与
    OR   = AND  + AND指令条数 , OR指令条数   =  4, // 位或
    XOR  = OR   + OR指令条数  , XOR指令条数  =  4, // 异或
    NAND = XOR  + XOR指令条数 , NAND指令条数 =  4, // 与非
    BT   = NAND + NAND指令条数, BT指令条数   =  2, // 位测试
    BTS  = BT   + BT指令条数  , BTS指令条数  =  2, // 位测试后位置位
    BTR  = BTS  + BTS指令条数 , BTR指令条数  =  2, // 位测试后位复位
    BTN  = BTR  + BTR指令条数 , BTN指令条数  =  2, // 位测试后位取反
    JIA  = BTN  + BTN指令条数 , JIA指令条数  =  2, // 当产生无符号大于标志时跳转
    JAE  = JIA  + JIA指令条数 , JAE指令条数  =  2, // 当产生无符号大于等于标志时跳转
    JIG  = JAE  + JAE指令条数 , JIG指令条数  =  2, // 当产生有符号大于标志时跳转
    JGE  = JIG  + JIG指令条数 , JGE指令条数  =  2, // 当产生右符号大于等于标志时跳转
    JIE  = JGE  + JGE指令条数 , JIE指令条数  =  2, // 当产生等于标志时跳转
    JIZ  = JIE                                   ,
    JNE  = JIE  + JIE指令条数 , JNE指令条数  =  2, // 当未产生等于标志时跳转
    JNZ  = JNE                                   ,
    JIC  = JNE  + JNE指令条数 , JIC指令条数  =  2, // 当产生进位标志时跳转
    JNC  = JIC  + JIC指令条数 , JNC指令条数  =  2, // 当未产生进位标志时跳转
    JMP  = JNC  + JNC指令条数 , JMP指令条数  =  2, // 无条件跳转
    JAL  = JMP  + JMP指令条数 , JAL指令条数  =  2, // 子过程调用
    MOV  = JAL  + JAL指令条数 , MOV指令条数  =  2, // 赋值
    SWP  = MOV  + MOV指令条数 , SWP指令条数  =  1, // 交换
    PSH  = SWP  + SWP指令条数 , PSH指令条数  =  1, // 压栈
    PSHS = PSH  + PSH指令条数 , PSHS指令条数 =  1, // 状态寄存器压栈
    POP  = PSHS + PSHS指令条数, POP指令条数  =  1, // 退栈
    POPS = POP  + POP指令条数 , POPS指令条数 =  1, // 状态寄存器退栈
    RET  = POPS + POPS指令条数, RET指令条数  =  1, // 返回

    DOWN = 0                                     , // 向上跳转
    UP   = 1                                     , // 向下跳转
};

结构体 内存修饰符{
    U08 包含T : 1;
    U08 位宽  : 2;
    U08 写入  : 1;

    模板<类 甲>
    内存修饰符(甲 OPC){
        复制(此, OPC);
    }
};

枚举 F2{
    AI2,
    AB2,
};

枚举 F4{
    ABI4,
    AAB4,
    ABT4,
    TAB4,
};

枚举 F8{
    AAI8,
    ABI8,
    AIA8,
    AIB8,
    AAB8,
    ABT8,
    ATB8,
    TAB8,
};

枚举 REG{
    SP = 0,
    RT = 1,
    R0 = SP,
    R1 = RT,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    RA,
    RB,
    RC,
    RD,
    RF,
};

结构体 INS{
    U08     A   : 4;
    U08     B   : 4;
    结构体 OPC结构{
        空 运算重载 = (U08 值){
            复制(C, 值);
        }
        运算重载 U08(){
            回递 U08(C);
        }
        指令区间 C;
    } OPC;

    INS() {}
    INS(U08 OPC) : 
        INS(OPC, 0){
    }
    INS(U08 OPC, U08 AB) {
        此.OPC = OPC;
        此.A = AB & 0xf;
        此.B = AB >> 4;
    }
    INS(U08 OPC, U08 A, U08 B){
        此.OPC = OPC;
        此.A = A;
        此.B = B;
    }

    静态 符号常量 U08 T = 1;
    运算重载 U16() {
        回递 *(U16 *)(此指针);
    }
};

结构体 处理器{
    枚举{
        整数器个数     = 16,
        指令个数       = 256,
        SP             = 0,
    };

    联合体 整数器{
        U64 U;
        I64 I;
        整数器() :I{} {}
        整数器(I64 I) : I(I){}
        整数器(U64 U) : U(U){}
    };

    结构体 立即数{
        立即数(){
            位宽 = 0;
            值 = 0;
        }
        空 装载(U08 W, U16 V){
            值 |= I64(V) << 位宽;
            位宽 += W;
        }
        运算重载 整数器(){
            回递 整数器(运算重载 I64());
        }
        运算重载 I64(){
            布尔 状态 = 指令集::位测试(U64(值), 位宽 - 1);
            I64  T = 值;
            若 (状态 == 置位态){
                值 |= U64(-1) << 位宽;
                T = 值;
            }
            值 = 0;
            位宽 = 0;
            回递 T;
        }
        I64 值;
        U08 位宽;
    };

    结构体 标志位{
        U64  CF : 1;
        U64  OF : 1;
        U64  ZF : 1;
        U64  AB : 1;
        U64  GT : 1;
    };

    结构体 段偏式{
        U32 OFS;            //偏移
        U32 SEG;            //段
        运算重载 U64 & () {
            回递 *(U64 *)此指针;
        }
        空 运算重载 = (U64 值){
            此.运算重载 U64 & () = 值;
        }
    };

    定义 空 (处理器::* 指令)();

    结构体 命令集{
        指令 函数;

        模板<类 匿名函数>
        空 运算重载 = (匿名函数 回调){
            复制(函数, & 匿名函数::运算重载());
        }

        空 执行(处理器 * 指针) {
            处理器 * 匿名参数化 = (处理器 *)(& 指针); //别问为啥, C++ Lambda是这样实现的
            (匿名参数化->*函数)();
        }
    };


    命令集 SET[指令个数];     //命令集合
    整数器 R[整数器个数];     //通用整数器
    整数器 TMP;               //临时寄存器
    立即数 IMM;               //立即数寄存器,读取后清零
    标志位 STA;               //状态寄存器
    段偏式 PC;                //程序计数器
    段偏式 PCS;               //调用栈
    INS    CMD;               //当前指令
    U08 *  MEM;               //内存起始地址
    U64    BYTES;             //总内存字节数

    #define $M(指令)     循环(自然数 __ = 指令; __ < 指令 + 自然数(指令 ## 指令条数); __++) SET[__] = [此指针]()
    #define $X(...)      [此指针](整数器 & A, 整数器 B, 整数器 C, U08 __VA_ARGS__)
    #define $J(条件)     IMM.装载(9, CMD & 0x1ff); U32 OFFSET = U32(IMM) << 1; 若 (条件){ PC.OFS += OFFSET; }
    #define $F1(类型,比较符,移位符,测试位,...)                         \
            若 (C.U 比较符 空间量(整数器)){                            \
                A.U    = 0;                                            \
                STA.CF = 0;                                            \
                STA.ZF = 1;                                            \
            }                                                          \
            非{                                                        \
                A.类型 = B.类型 移位符 C.U __VA_ARGS__;                \
                STA.CF = C.U ? 指令集::位测试(B.U, (测试位)) : 0;      \
                STA.ZF = A.类型 == 0;                                  \
            }

    // FAKE
    空 内存写入(空指针 A, U64 B, U64 偏移, U08 字节数) {
        推导类型 D = (U08 *)MEM + B + 偏移;
        推导类型 S = (U08 *)A;
        只要(字节数--) {
            D[0] = S[0];
            D   += 1;
            S   += 1;
        }
    }
    
    // FAKE
    空 内存读取(空指针 A, U64 B, U64 偏移, U08 字节数) {
        推导类型 S = (U08 *)MEM + B + 偏移;
        推导类型 D = (U08 *)A;
        只要(字节数--) {
            D[0] = S[0];
            D   += 1;
            S   += 1;
        }
    }
    
    模板<类 运算回调>
    空 二相运算(运算回调 回调) {
        U08  F = (CMD.OPC & 0x2) != 0;
        开关(F2(CMD.OPC & 0x1)) {
        通向 AI2: IMM.装载(4, CMD.B);
                  回调(R[CMD.A], IMM     , 整数器(0ull), F); 跳出;
        通向 AB2: 回调(R[CMD.A], R[CMD.B], 整数器(0ull), F); 跳出;
        }
    }

    模板<类 运算回调>
    空 四相运算(运算回调 回调) {
        U08  F = (CMD.OPC & 0x4) != 0;
        开关(F4(CMD.OPC & 0x3)) {
        通向 ABI4: 回调(R[CMD.A], R[CMD.B], IMM     , F); 跳出;
        通向 AAB4: 回调(R[CMD.A], R[CMD.A], R[CMD.B], F); 跳出;
        通向 ABT4: 回调(R[CMD.A], R[CMD.B], R[CMD.T], F); 跳出;
        通向 TAB4: 回调(R[CMD.T], R[CMD.A], R[CMD.B], F); 跳出;
        }
    }

    模板<类 运算回调>
    空 八相运算(运算回调 回调) {
        U08  F = (CMD.OPC & 0x8) != 0;
        开关(F8(CMD.OPC & 0x7)) {
        通向 AAI8: IMM.装载(4, CMD.B);
                   回调(R[CMD.A], R[CMD.A], IMM     , F); 跳出;
        通向 ABI8: 回调(R[CMD.A], R[CMD.B], IMM     , F); 跳出;
        通向 AIA8: 回调(R[CMD.A], IMM     , R[CMD.A], F); 跳出;
        通向 AIB8: 回调(R[CMD.A], IMM     , R[CMD.B], F); 跳出;
        通向 AAB8: 回调(R[CMD.A], R[CMD.A], R[CMD.B], F); 跳出;
        通向 ABT8: 回调(R[CMD.A], R[CMD.B], R[CMD.T], F); 跳出;
        通向 ATB8: 回调(R[CMD.A], R[CMD.T], R[CMD.B], F); 跳出;
        通向 TAB8: 回调(R[CMD.T], R[CMD.A], R[CMD.B], F); 跳出;
        }
    }

    处理器(空指针 MEM, U64 BYTES){
        清空(此);
        此.MEM   = (U08 *)MEM;
        此.BYTES = (U64)BYTES;
        $M(PSI){
            IMM.装载(12, CMD & 0xfff);
        };
        $M(MRW){
            内存修饰符 F = CMD.OPC;
            U64     &  A = R[CMD.A].U;
            U64        B = R[CMD.B].U;
            U64        C = F.包含T ? R[CMD.T].U + IMM : IMM;
            U08        字节数 = 1 << F.位宽;

            若 (F.写入){
                内存写入(& A, B, C, 字节数);
            }
            非{
                A = 0;
                内存读取(& A, B, C, 字节数);
            }
        };
        $M(SUB){
            八相运算($X(带借位){
                STA.ZF = B.I == C.I;
                STA.GT = B.I >  C.I;
                STA.AB = B.U >  C.U;
                STA.CF = 指令集::减法(A.U, B.U, C.U, STA.CF & 带借位);
            });
        };
        $M(DIV){
            八相运算($X(有符号){
                若 (STA.OF = (C.U == 0)){
                    回递;
                }
                若 (有符号){
                    TMP.I = 指令集::除法(A.I, B.I, C.I);
                }
                非{
                    TMP.U = 指令集::除法(A.U, B.U, C.U);
                }
                STA.CF = TMP.U != 0;
                STA.ZF = A.U == 0;
            });
        };
        $M(SHL){
            八相运算($X(带进位){
                $F1(U, >, <<, 空间量(C) - C.U, | STA.CF);
            });
        };
        $M(SHR){
            八相运算($X(){
                $F1(U, >, >>, C.U - 1);
            });
        };
        $M(SAR){
            八相运算($X(){
                $F1(I, >=, >>, C.U - 1);
            });
        };
        $M(ADD){
            四相运算($X(带进位){
                STA.CF = 指令集::加法(A.U, B.U, C.U, STA.CF & 带进位);
                STA.ZF = A.U == 0;
            });
        };
        $M(MUL){
            四相运算($X(有符号) {
                若 (有符号){
                    TMP.I  = 指令集::乘法(A.I, B.I, C.I);
                }
                非{
                    TMP.U  = 指令集::乘法(A.U, B.U, C.U);
                }
                STA.CF = TMP.U != 0;
                STA.ZF = A.U == 0;
            });
        };
        $M(AND){
            四相运算($X() {
                A.U = B.U & C.U;
                STA.ZF = A.U == 0;
            });
        };
        $M(OR){
            四相运算($X() {
                A.U = B.U | C.U;
                STA.ZF = A.U == 0;
            });
        };
        $M(XOR){
            四相运算($X() {
                A.U = B.U ^ C.U;
                STA.ZF = A.U == 0;
            });
        };
        $M(NAND){
            四相运算($X() {
                A.U = ~(B.U & C.U);
                STA.ZF = A.U == 0;
            });
        };
        $M(BT){
            二相运算($X() {
                STA.CF = 指令集::位测试(A.U, B.U);
            });
        };
        $M(BTS){
            二相运算($X() {
                STA.CF = 指令集::位测试后位置位(A.U, B.U);
            });
        };
        $M(BTR){
            二相运算($X() {
                STA.CF = 指令集::位测试后位复位(A.U, B.U);
            });
        };
        $M(BTN){
            二相运算($X() {
                STA.CF = 指令集::位测试后位取反(A.U, B.U);
            });
        };
        $M(JIA){
            $J(STA.AB);
        };
        $M(JAE){
            $J(STA.AB | STA.ZF);
        };
        $M(JIG){
            $J(STA.GT);
        };
        $M(JGE){
            $J(STA.GT | STA.ZF);
        };
        $M(JIE){
            $J(STA.ZF);
        };
        $M(JNE){
            $J(STA.ZF == 否);
        };
        $M(JIC){
            $J(STA.CF);
        };
        $M(JNC){
            $J(STA.CF == 否);
        };
        $M(JMP){
            $J(是);
        };
        $M(JAL){
            U32 OFFSET;
            若 (CMD.OPC & AB2){
                IMM.运算重载 I64();
                OFFSET = U32(R[CMD.B].U);
            }
            非{
                IMM.装载(8 + 1, 0 << 8 | CMD & 0xff);
                OFFSET = 0xffffffff & IMM;
            }
            内存写入(& PC.OFS, PCS, 0, 空间量(PC.OFS));
            PCS   += 空间量(PC.OFS);
            PC.OFS = U32(OFFSET - 空间量(CMD));
        };
        $M(MOV){
            二相运算($X() {
                A.U = B.U;
            });
        };
        $M(SWP){
            推导类型 A = R[CMD.A];
            推导类型 B = R[CMD.B];
            R[CMD.A] = A;
            R[CMD.B] = B;
        };
        $M(PSH){
            若 (CMD.A <= CMD.B) {
                循环(整数 I = CMD.A; I <= 整数(CMD.B); I++) {
                    内存写入(& R[I].U, R[SP].U, 0, 空间量(整数器));
                    R[SP].U += 空间量(整数器);
                }
            }
            非 {
                循环(整数 I = CMD.A + 1; I-- > 整数(CMD.B);) {
                    内存写入(& R[I].U, R[SP].U, 0, 空间量(整数器));
                    R[SP].U += 空间量(整数器);
                }
            }
        };
        $M(POP){
            若 (CMD.A <= CMD.B) {
                循环(整数 I = CMD.A; I <= 整数(CMD.B); I++) {
                    R[SP].U -= 空间量(整数器);
                    内存读取(& R[I].U, R[SP].U, 0, 空间量(整数器));
                }
            }
            非 {
                循环(整数 I = CMD.A + 1; I-- > 整数(CMD.B);) {
                    R[SP].U -= 空间量(整数器);
                    内存读取(& R[I].U, R[SP].U, 0, 空间量(整数器));
                }
            }
        };
        $M(PSHS) {
            内存写入(& STA, R[SP].U, 0, 空间量(整数器));
            R[SP].U += 空间量(整数器);
            静态断言(空间量(整数器) == 空间量(STA), "ERROR");
        };
        $M(POPS) {
            R[SP].U -= 空间量(整数器);
            内存读取(& STA, R[SP].U, 0, 空间量(整数器));
            静态断言(空间量(整数器) == 空间量(STA), "ERROR");
        };
        $M(RET){
            PCS -= 空间量(PC);
            内存读取(& PC, PCS, 0, 空间量(PC));
        };
    }

    空 运行() {
        只要(是) {
            内存读取(& CMD, PC, 0, 空间量(CMD));
            SET[CMD.OPC].执行(此指针);
            PC += 空间量(CMD);
        }
    }
};

#undef $M
#undef $X
#undef $J
#undef $F1

