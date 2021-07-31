# Mix-C ISA
```
cifxx 条件跳转
bit15....................................................bit00
iiii        BB          AA          bb      cc          0000
4bit        2bit        2bit        2bit    2bit        4bit
相对跳转    操作数B     操作数A     bank    跳转条件    opcode

cc
- 00    cifeq       操作数A[等于    ]操作数B时继续执行否则跳转
- 01    cifne       操作数A[不等    ]操作数B时继续执行否则跳转
- 10    cifle       操作数A[小于等于]操作数B时继续执行否则跳转
- 11    ciflt       操作数A[小于    ]操作数B时继续执行否则跳转

备注:
限制
- 只支持寄存器间比较
- 操作数A 与 B 必须属于同一 bank, reg0~reg3/reg4~reg7/reg8~reg11/reg12~reg15 各为一组

ifxx 跳转
bit15....................................................bit00
iiii_iiii                                   cccc        0001
8bit                                        4bit        4bit
相对跳转                                    跳转条件    opcode

cc
- 0x0   ifeq        状态寄存器[eq=1        ]时继续执行否则跳转
- 0x1   ifne        状态寄存器[eq=0        ]时继续执行否则跳转
- 0x2   ifle        状态寄存器[eq=1 or gt=0]时继续执行否则跳转
- 0x3   iflt        状态寄存器[gt=0        ]时继续执行否则跳转
- 0x4   ifge        状态寄存器[eq=1 or gt=1]时继续执行否则跳转
- 0x5   ifgt        状态寄存器[gt=1        ]时继续执行否则跳转
- 0x6   ifzf        状态寄存器[zf=1        ]时继续执行否则跳转
- 0x7   ifnz        状态寄存器[zf=0        ]时继续执行否则跳转
- 0x8   ifcf        状态寄存器[cf=1        ]时继续执行否则跳转
- 0x9   ifnc        状态寄存器[cf=0        ]时继续执行否则跳转
- 0xa   ifuf        状态寄存器[uf=1        ]时继续执行否则跳转
- 0xb   ifnu        状态寄存器[uf=0        ]时继续执行否则跳转
- 0xc   jmp         强制跳转
- 0xd   ret         子过程返回
- 0xe   jali        通过立即数调用子过程
- 0xf   jalr        通过寄存器调用子过程
```

