#ifndef xpack_extern_isa_cpu
#define xpack_extern_isa_cpu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_isa_cpu::origin::inc
#include"configure/platform.hpp"
#include"macro/xdefer.hpp"
#include"math/const.hpp"
#include"memop/cast.hpp"
#include"memop/signature.hpp"
#include"memop/swap.hpp"
#include"meta/is_float.hpp"
#include"meta/is_integer.hpp"
#include"meta/is_signed.hpp"
#include"instruction/add.hpp"
#include"instruction/index_of_last_set.hpp"
#include"instruction/mul.hpp"
#include"instruction/shift_left.hpp"
#include"instruction/shift_right.hpp"
#include"instruction/sub.hpp"
#include"mixc.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_isa_cpu::origin{
    enum cmd_t : u08{
        // 带比较的转移指令
        cifeq           ,
        cifne           = cifeq  + 4,
        cifle           = cifne  + 4,
        ciflt           = cifle  + 4,

        // 转移指令
        ifeq            = ciflt  + 4,
        ifne            ,
        ifle            ,
        iflt            ,
        ifge            ,
        ifgt            ,
        ifzf            ,
        ifnz            ,
        ifcf            ,
        ifnc            ,
        ifof            ,
        ifno            ,
        jmp             ,
        ret             ,
        jali            ,
        jalr            ,

        // 广播式赋值
        bdcss           ,
        bdcsi           = bdcss  + 4,
        bdcff           = bdcsi  + 4,
        bdcfi           = bdcff  + 4,
        bdcqq           = bdcfi  + 4,
        bdcqi           = bdcqq  + 4,
        bdcqqx          = bdcqi  + 4,
        bdcqix          = bdcqqx + 4,

        // 赋值指令
        // u64 <- u08/u16/u32/u64
        // i64 <- i08/i16/i32/i64
        movqb           = bdcqix + 4,
        movqbx          ,
        movqw           ,
        movqwx          ,
        movqd           ,
        movqdx          ,
        movqq           ,
        movqqx          ,

        movss           ,
        movsf           ,
        movsq           ,
        movsqx          ,
        movfs           ,
        movff           ,
        movfq           ,
        movfqx          ,

        movqs           ,
        movqsx          ,
        movqf           ,
        movqfx          ,
        movsi           ,
        movsix          ,
        movfi           ,
        movfix          ,

        // 读取用户扩展寄存器
        rduxr           ,

        // 读取特权扩展寄存器
        rdpri           = rduxr  + 2,

        // 写入用户扩展寄存器
        wruxr           = rdpri  + 2,

        // 写入特权扩展寄存器
        wrpri           = wruxr  + 2,

        // 读取内存
        // u64 <- mu08/mu16/mu32/mu64
        // i64 <- mi08/mi16/mi32/mi64
        ldb             = wrpri  + 2,
        ldbx            = ldb    + 2,
        ldw             = ldbx   + 2,
        ldwx            = ldw    + 2,
        ldd             = ldwx   + 2,
        lddx            = ldd    + 2,
        ldq             = lddx   + 2,
        ldqx            = ldq    + 2,

        // 读取栈内存
        ldkq            = ldqx   + 2,
        ldkqx           = ldkq   + 2,
        ldks            = ldkqx  + 2,
        ldkf            = ldks   + 2,

        // 内存读取
        // f32 <- mf32
        // f64 <- mf64
        lds             = ldkf   + 2,
        ldf             = lds    + 2,

        // 写入内存
        // u08/i08      -> mu08
        // u16/i16      -> mu16
        // u32/i32/f32  -> mu32
        // u64/i64/f64  -> mu64
        stb             = ldf    + 2 + 2 * 2, // 保留 2x2 空位
        stw             = stb    + 2,
        std             = stw    + 2,
        stq             = std    + 2,

        // 写入栈内存
        stkq            = stq    + 2, 

        // 基础逻辑运算
        band            = stkq   + 2 + 3 * 2, // 保留 3x2 空位
        bor             = band   + 4,
        bxor            = bor    + 4,
        bnand           = bxor   + 4,

        // 算术运算
        add             = bnand  + 4,
        sub             = add    + 8,
        mul             = sub    + 8,
        div             = mul    + 8,
        shr             = div    + 8,
        shl             = shr    + 8,

        // 比较
        cmp             = shl    + 8,

        // 位操作
        bop             = cmp    + 4,

        // 最小最大值
        miax            = bop    + 4,

        // 交换两个值
        swap            ,

        // 内部互斥锁
        lock            ,

        // 调试中断
        brk             ,

        // 立即数加载
        imm             = 256 - 16,
    };

    enum rduxr_t{
        rdst            , // 读取临时 f32 结果寄存器
        rdft            , // 读取临时 f64 结果寄存器
        rdsta           , // 读取状态寄存器
        rdmod           , // 预定除法余数
        rdproh          , // 预定乘法高位
        rdsfto          , // 预定移位溢出位组
        rdtms           , // 读取时间戳
    };

    enum wruxr_t{
        wrst            , // 写入临时 f32 结果寄存器
        wrft            , // 写入临时 f64 结果寄存器
        wrsta           , // 写入状态寄存器
    };

    enum bop_t{
        // ra   , ra
        // ra   , rt
        // rt   , ra
        // ra^1 , ra
        lsb             ,            // 最低置位位
        hsb             = lsb   + 4, // 最高置位位
        clb             = hsb   + 4, // 清除最低置位位
        chb             = clb   + 4, // 清除最高置位位
        sbc             = chb   + 4, // 置位位个数
    };

    enum f4_t{
        f4aab,
        f4abt,
        f4tab,
        f4tai,
    };

    enum c4_t{
        c4ab,
        c4ai,
        c4ia,
        c4at,
    };

    enum f8_t{
        f8abt,
        f8atb,
        f8aab,
        f8tab,
        f8aai,
        f8aia,
        f8tai,
        f8tia,
    };

    typedef enum register_state_t{
        is_f32,
        is_f64,
        is_u64,
        is_i64,
    } res_t;

    enum : uxx{
        general_purpose_register_count  = 0x10,
        no_predetermined                = 0x10,
        over_area                       = 0x1,  // 跨区域跳转
    };

    struct cifxx_t{
        u08 opc     : 4;
        u08 mode    : 2;
        u08 bank    : 2;
        u08 opa     : 2;
        u08 opb     : 2;
        u08 im4     : 4;
    };

    struct jalx_t{
        u08             : 8;
        u08   opt       : 4;
        u08   im4_opa   : 4;
    };

    enum opt_t : uxx{ 
        save_lreg_type  = 0x1,  // 保存类型寄存器低 8 组
        save_hreg_type  = 0x2,  // 保存类型寄存器高 8 组
        save_op_state   = 0x4,  // 保存运算状态寄存器
        save_pstate     = 0x8,  // 保存预设寄存器
    };

    struct bdc_t{
        u08         : 6;
        u08   bank  : 2;
        u08   bmp   : 4;
        u08   opb   : 4;
    };

    struct ldx_t{
        u08                : 4;
        u08    scale       : 2;
        u08    sign_extern : 1;
        u08    with_rt     : 1;
    };

    struct rwss_t{
        u08                 : 5;
        u08     type        : 2;
        u08     side_effect : 1;
        u08     opa         : 4;
        u08     im4         : 4;
    };

    struct stxx_t{
        u08                 : 5;
        u08     scale       : 2;
        u08     with_rt     : 1;
    };

    struct cpu_t{
    public:
        cpu_t(uxx bytes) : 
            ram(inc::alloc<u08>(inc::memory_size{bytes})){

            #define xgen(start,end,func)                                    \
                for(uxx i = uxx(cmd_t::start); i <= uxx(cmd_t::end); i++)   \
                    cmd[i] = the.cast(& cpu_t::func);

            xgen(cifeq, ciflt   , asm_cifxx)
            xgen(ifeq , jmp     , asm_ifxx)
            xgen(ret  , ret     , asm_ret)
            xgen(jali , jalr    , asm_jalx)
            xgen(bdcss, bdcqix  , asm_bdc)
            xgen(movqb, movfix  , asm_mov)

            // xgen(rduxr, rduxr + 1, )
            // xgen(rdpri, rdpri + 1, )
            // xgen(wruxr, wruxr + 1, )
            // xgen(wrpri, wrpri + 1, )

            xgen(ldb  , ldqx    , asm_ldxx)
            xgen(ldkq , ldkf    , asm_ldkxx)
            xgen(lds  , ldf     , asm_ldxx)
            xgen(stb  , stq     , asm_stxx)
            xgen(stkq , stkq  +2, asm_stxx)
            xgen(band , band  +4, asm_band)
            xgen(bor  , bor   +4, asm_bor)
            xgen(bxor , bxor  +4, asm_bxor)
            xgen(bnand, bnand +4, asm_bnand)
            xgen(add  , add   +8, asm_add)
            xgen(sub  , sub   +8, asm_sub)
            xgen(mul  , mul   +8, asm_mul)
            xgen(div  , div   +8, asm_div)
            xgen(shr  , shr   +8, asm_shr)
            xgen(shl  , shl   +8, asm_shl)
            xgen(cmp  , cmp   +8, asm_cmp)

            // MORE TODO:===================================

            xgen(imm,   imm  +16, asm_imm)

            #undef  xgen
        }

        void run(){
            while(true){
                // 取指令
                rdmem(& ins, pc.address, sizeof(ins));

                // 执行
                exec();

                // 程序计数器指向一下条指令
                pc.address += sizeof(ins);
            }
        }

    private:
        using the_t     = cpu_t;

        struct imm_t{
            auto & load(uxx value, uxx bits){
                imm         |= value << total_bits;
                total_bits  += bits;
                return *this;
            }

            template<class type>
            type read_with_clear(){
                // 返回时执行
                xdefer{
                    imm         = 0;
                    total_bits  = 0;
                };

                if constexpr (inc::is_float<type>){
                    auto bits = sizeof(type) * 8;
                    if (total_bits < bits){
                        imm <<= bits - total_bits;
                    }
                    return inc::cast<type>(imm);
                }
                //  inc::is_integer<type>
                else{
                    if (total_bits == 0){
                        return 0;
                    }
                    if (inc::index_of_last_set(imm) == total_bits - 1 and 
                       (inc::is_signed<type> or total_bits > 4) and total_bits <= 64){
                        imm |= u64(-1) << total_bits;
                    }
                    return type(imm);
                }
            }
        private:
            u64 imm         = 0;
            uxx total_bits  = 0;
        };

        union reg_t{
            u08     ru08;
            u16     ru16;
            struct{
                u32 ru32;
                u32 rh32;
            };
            u64     ru64;
            i08     ri08;
            i16     ri16;
            i32     ri32;
            i64     ri64;
            f32     rf32;
            f64     rf64 = 0;

            reg_t(){}
            reg_t(f32 v){
                rf32 = (v);
            }
            reg_t(f64 v){
                rf64 = (v);
            }
            reg_t(u64 v){
                ru64 = (v);
            }
            reg_t(i64 v){
                ri64 = (v);
            }
        };

        struct ins_t{
            u08     opc;                // op-code

            union{
                struct{
                    u08 opa : 4;        // 操作数 a
                    u08 opb : 4;        // 操作数 b
                };
                struct{
                    u08     : 4;
                    u08 im4 : 4;        // 4bit 立即数，与 opb 共用位域
                };
                u08 im8;                // 8bit 立即数
            };

            enum{ opt = 15 };           // 临时寄存器的编号
        };

        typedef struct reg_type_group{
            struct bits_delegate{
                bits_delegate(u32p bits, uxx offset) : 
                    bits(bits), offset(offset){
                }

                operator register_state_t(){
                    return register_state_t(
                        (*bits >> offset) & 0x3
                    );
                }

                register_state_t operator=(register_state_t value){
                    *bits &= ~(0x3   << offset);
                    *bits |=  (value << offset);
                    return value;
                }
            private:
                u32p bits;
                uxx  offset;
            };

            bits_delegate operator [](uxx index){
                return bits_delegate(xref type_group, index * 2);
            }
        private:
            u32 type_group;
        } rtg_t;

        struct sta_t{
            // 寄存器类型 2bit * 16
            // - 00 f32
            // - 01 f64
            // - 10 u64
            // - 11 i64
            rtg_t   reg_type;

            // 大于
            u16     gt                  : 1;

            // 等于
            u16     eq                  : 1;

            // 零标志
            u16     zf                  : 1;

            // 上溢/进位/借位
            u16     cf                  : 1;

            // 下溢
            u16     of                  : 1;

            // 保留
            u16                         : 11;

            // 保留
            u16                         : 1;

            // 预设(predetermined)
            // 指定余数存放的寄存器
            // 在指定寄存器获取到余数后该位域被复位为 no_predetermined
            u16     pmod                : 5 = no_predetermined;

            // 指定乘法高位积存放的寄存器
            // 在指定寄存器获取到高位积后该位域被复位为 no_predetermined
            u16     pmulh               : 5 = no_predetermined;

            // 指定移位溢出位存放的寄存器
            // 在指定寄存器获取到溢出位后该位域被复位为 no_predetermined
            u16     psfto               : 5 = no_predetermined;
        };

        // 段偏式
        union seg_t{
            struct{
                u16 position;
                u16 area;
            };

            struct{
                u32 offset;
                u32 segment;
            };

            u64     address;

            seg_t(){}
            seg_t(u64 address) : 
                address(address){
            }
        };

        struct regs_t{
            using the_t = regs_t;

            reg_t & operator[](uxx index){
                if (index != ins_t::opt or 
                    the->mode[index] == is_u64 or 
                    the->mode[index] == is_i64){
                    return regs[index];
                }
                if (the->mode[ins_t::opt] == is_f32){
                    return regs[index + 1];
                }
                else{
                    return regs[index + 2];
                }
            }
        private:
            cpu_t * operator->(){
                auto offset_ptr = & cpu_t::regs;
                auto offset     = * uxxp(& offset_ptr);
                return (cpu_t *)((u08p)this - offset);
            }

            reg_t   regs[general_purpose_register_count + 2/*f32.rt + f64.rt*/];
        };

        u08p    ram;                                    // 内存起始地址
        imm_t   rim;                                    // 立即数寄存器
        ins_t   ins;                                    // 指令寄存器
        regs_t  regs;                                   // 通用寄存器组
        sta_t   sta;                                    // 状态寄存器
        seg_t   pc;                                     // 程序计数器
        seg_t   cs;                                     // 调用栈寄存器
        seg_t   ss;                                     // 堆栈寄存器
        rtg_t & mode    = sta.reg_type;                 // 寄存器类型
        voidp   cmd[256];                               // 指令集清单

        void exec(){
            union {
                voidp           mem;
                void (       *  gnu_call)(const void *);
                void (the_t::*  msvc_call)();
            } u;

            u.mem = cmd[ins.opc];

            #if xis_msvc_native
            return (this->*u.msvc_call)();
            #else
            return u.gnu_call(this);
            #endif
        }

        static voidp cast(void(cpu_t::* func)()){
            union {
                voidp          mem;
                void (cpu_t::* call)();
            } u;

            u.call = func;
            return u.mem;
        }

        static i64 sign_extern(reg_t val, uxx scale){
            auto idx  = 1ull << ((1ull << scale) * 8);
            auto test = idx >> 1;
            auto mask = idx - 1;

            // zero extern : sign extern
            val.ri64 = val.ri64 & test ? val.ri64 & mask : val.ri64 | ~mask;
            return val.ri64;
        }

        static i64 zero_extern(reg_t val, uxx scale){
            auto idx  = 1ull << ((1ull << scale) * 8);
            auto mask = idx - 1;

            // zero extern
            val.ri64 = val.ri64 & mask;
            return val.ri64;
        }

        f80 unified_reg(uxx index){
            switch(mode[index]){
            case res_t::is_f32: return f80(regs[index].rf32);
            case res_t::is_f64: return f80(regs[index].rf64);
            case res_t::is_u64: return f80(regs[index].ru64);
            // case res_t::is_i64: 
            default:            return f80(regs[index].ri64);
            }
        }

        f80 unified_imm(res_t mode){
            switch(mode){
            case res_t::is_f32: return f80(rim.read_with_clear<f32>());
            case res_t::is_f64: return f80(rim.read_with_clear<f64>());
            case res_t::is_u64: return f80(rim.read_with_clear<u64>());
            // case res_t::is_i64: 
            default:            return f80(rim.read_with_clear<i64>());
            }
        }

        void cmp(c4_t cmode, uxx ia, uxx ib){
            f80 a   = unified_reg(ia);
            f80 b;

            // 寄存器之间比较
            if (cmode == c4ab){
                b   = unified_reg(ib);
            }
            // 寄存器与立即数
            else{
                rim.load(ib, 4/*bits*/);
                b   = unified_imm(mode[ia]);
            }

            sta.eq  = a == b;
            sta.gt  = cmode == c4ia ? a < b : a > b;
        }

        void asm_cifxx(){
            auto ins = inc::cast<cifxx_t>(the.ins);
            cmp(c4_t::c4ab, ins.bank << 2 | ins.opa, ins.bank << 2 | ins.opb);

            switch(rim.load(ins.im4, 4/*bits*/); cmd_t(ins.mode | cifeq)){
            case cifeq: ifxx(    sta.eq);               break;
            case cifne: ifxx(not sta.eq);               break;
            case cifle: ifxx(    sta.eq or not sta.gt); break;
            // case ciflt: 
            default:    ifxx(not sta.eq or not sta.gt); break;
            }
        }

        void ifxx(bool contiguous){
            i64 offset      = rim.read_with_clear<i64>(); 

            // 不满足条件就跳转
            // 向上跳转（回跳）
            // 向下跳转
            if (not contiguous){
                pc.offset  += u32(offset >= 0 ? offset + 1 : offset - 2) * sizeof(ins_t);
            }
        }

        void asm_ifxx(){
            switch(rim.load(ins.im8, 8/*bits*/); cmd_t(ins.opc)){
            case cmd_t::ifeq: ifxx(    sta.eq);               break;
            case cmd_t::ifne: ifxx(not sta.eq);               break;
            case cmd_t::ifle: ifxx(    sta.eq or not sta.gt); break;
            case cmd_t::iflt: ifxx(not sta.eq or not sta.gt); break;
            case cmd_t::ifge: ifxx(    sta.eq or     sta.gt); break;
            case cmd_t::ifgt: ifxx(not sta.eq or     sta.gt); break;
            case cmd_t::ifzf: ifxx(    sta.zf);               break;
            case cmd_t::ifnz: ifxx(not sta.zf);               break;
            case cmd_t::ifcf: ifxx(    sta.cf);               break;
            case cmd_t::ifnc: ifxx(not sta.cf);               break;
            case cmd_t::ifof: ifxx(    sta.of);               break;
            case cmd_t::ifno: ifxx(not sta.of);               break;
            // case cmd_t::jmp : 
            default:          ifxx(false/*force*/);           break;
            }
        }

        void asm_ret(){
            // 跨程序段跳转
            // TODO====================================================================================

            cs.address             -= 2;
            rdmem(& pc.position, cs.address, 2/*bytes*/);

            if (pc.position & over_area){
                pc.position        &= ~over_area;
                cs.address         -= 2;
                rdmem(& pc.area, cs.address, 2/*bytes*/);
            }

            // 读取 jal 指令信息
            auto ins                = jalx_t{};
            rdmem(& ins, pc.address, sizeof(ins_t));

            // 恢复调用时保存的信息
            for(uxx i = 4; i-- > 0;){
                if (u16 & v = u16p(& sta)[i]; ins.opt & (1 << i)){
                    cs.address     -= 2;
                    wrmem(& v, cs.address, 2/*bytes*/);
                }
            }
        }

        void asm_jalx(){
            auto ins                = inc::cast<jalx_t>(the.ins);
            auto target             = the.ins.opc == jali ? 
                rim.load(ins.im4_opa, 4/*bit*/).read_with_clear<u64>() :
                regs[ins.im4_opa].ru64;
            auto address            = seg_t(target & ~over_area);

            // 跨程序段跳转
            if (address.segment){
                // TODO====================================================================================
            }

            // 按需保存状态寄存器
            for(uxx i = 0; i < 4; i++){
                if (u16 v = u16p(& sta)[i]; ins.opt & (1 << i)){
                    wrmem(& v, cs.address, 2/*bytes*/);
                    cs.address     += 2;
                }
            }

            // 段内跨区域跳转
            // 让 position 排后边，作为第一个读到的 u16，在根据 over_area 位判断是否存在 area
            if (address.area){
                pc.position        |= over_area;
                wrmem(& pc.area, cs.address, 2/*bytes*/);
                pc.offset           = address.offset;
                cs.address         += 2;
            }

            wrmem(& pc.position, cs.address, 2/*bytes*/);
            pc.position             = address.position;
            cs.address             += 2;

            // 当执行 jalx 指令后，此时 pc 会指向子函数第一条指令
            // 接着在 the.run() 函数中完成一轮循环，需要将 pc += sizeof(ins_t)
            // 这里让 pc -= sizeof(ins_t); 保证下一条指令指向子函数第一条指令
            // 在 jalx() 中不直接执行子函数第一条指令，避免恶意的递归调用（第一条指令是 jalx 指令）导致爆栈
            pc.address             -= sizeof(ins_t);
        }

        void asm_imm(){
            auto im = inc::cast<u16>(ins) & 0xfff;
            rim.load(im, 12/*bits*/);
        }

        void asm_mov(){
            #define a   regs[ins.opa]
            #define b   regs[ins.opb]
            #define t   regs[ins.opt]

            switch(cmd_t(ins.opc)){
            case cmd_t::movqb : mode[ins.opa] = res_t::is_u64; a.ru64 = b.ru08; break;
            case cmd_t::movqbx: mode[ins.opa] = res_t::is_i64; a.ri64 = b.ri08; break;
            case cmd_t::movqw : mode[ins.opa] = res_t::is_u64; a.ru64 = b.ru16; break;
            case cmd_t::movqwx: mode[ins.opa] = res_t::is_i64; a.ri64 = b.ri16; break;
            case cmd_t::movqd : mode[ins.opa] = res_t::is_u64; a.ru64 = b.ru32; break;
            case cmd_t::movqdx: mode[ins.opa] = res_t::is_i64; a.ri64 = b.ri32; break;
            case cmd_t::movqq : mode[ins.opa] = res_t::is_u64; a.ru64 = b.ru64; break;
            case cmd_t::movqqx: mode[ins.opa] = res_t::is_i64; a.ri64 = b.ri64; break;

            case cmd_t::movss : mode[ins.opa] = res_t::is_f32; a.rf32 = b.rf32; a.rh32 = 0; break;
            case cmd_t::movsf : mode[ins.opa] = res_t::is_f32; a.rf32 = b.rf64; a.rh32 = 0; break;
            case cmd_t::movsq : mode[ins.opa] = res_t::is_f32; a.rf32 = b.ru64; a.rh32 = 0; break;
            case cmd_t::movsqx: mode[ins.opa] = res_t::is_f32; a.rf32 = b.ri64; a.rh32 = 0; break;
            case cmd_t::movfs : mode[ins.opa] = res_t::is_f64; a.rf64 = b.rf32; break;
            case cmd_t::movff : mode[ins.opa] = res_t::is_f64; a.rf64 = b.rf64; break;
            case cmd_t::movfq : mode[ins.opa] = res_t::is_f64; a.rf64 = b.ru64; break;
            case cmd_t::movfqx: mode[ins.opa] = res_t::is_f64; a.rf64 = b.ri64; break;

            case cmd_t::movqs : mode[ins.opa] = res_t::is_u64; a.ru64 = b.rf32; break;
            case cmd_t::movqsx: mode[ins.opa] = res_t::is_i64; a.ri64 = b.ru64; break;
            case cmd_t::movqf : mode[ins.opa] = res_t::is_u64; a.ru64 = b.rf64; break;
            case cmd_t::movqfx: mode[ins.opa] = res_t::is_i64; a.ri64 = b.ri64; break;

            case cmd_t::movsi : 
                mode[ins.opa] = res_t::is_f32;
                a.rf32 = (f32)rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>();
                a.rh32 = 0; 
                break;
            case cmd_t::movsix:
                mode[ins.opa] = res_t::is_f32;
                a.rf32 = (f32)rim.load(ins.im4, 4/*bits*/).read_with_clear<i64>();
                a.rh32 = 0; 
                break;
            case cmd_t::movfi :
                mode[ins.opa] = res_t::is_f64;
                a.rf64 = (f64)rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>();
                break;
            // case cmd_t::movfix:
            default:
                mode[ins.opa] = res_t::is_f64;
                a.rf64 = (f64)rim.load(ins.im4, 4/*bits*/).read_with_clear<i64>();
                break;
            }

            #undef a
            #undef b
            #undef t
        }

        void asm_bdc(){
            auto i      = inc::cast<bdc_t>(ins);
            auto assign = [&](res_t mode, auto value){
                for(uxx mask = i.bmp, idx = 0; mask != 0; mask >>= 1, idx++){
                    if (0 == (mask & 1)){
                        continue;
                    }

                    // 先设置模式，当设置的是临时寄存器的情况，内部会根据当前寄存器的类型进行选择
                    this->mode[i.bank << 2 | idx] = mode;
                    this->regs[i.bank << 2 | idx] = value;

                    if (mode != res_t::is_f32){
                        continue;
                    }

                    this->regs[i.bank << 2 | idx].rh32 = 0;
                }
            };

            switch(cmd_t(ins.opc)){
            case cmd_t::bdcss : assign(res_t::is_f32, regs[i.opb]);                                         break;
            case cmd_t::bdcsi : assign(res_t::is_f32, rim.load(ins.im4, 4/*bits*/).read_with_clear<f32>()); break;
            case cmd_t::bdcff : assign(res_t::is_f64, regs[ins.opb]);                                       break;
            case cmd_t::bdcfi : assign(res_t::is_f64, rim.load(ins.im4, 4/*bits*/).read_with_clear<f64>()); break;
            case cmd_t::bdcqq : assign(res_t::is_u64, regs[ins.opb]);                                       break;
            case cmd_t::bdcqi : assign(res_t::is_u64, rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>()); break;
            case cmd_t::bdcqqx: assign(res_t::is_i64, regs[ins.opb]);                                       break;
            // case cmd_t::bdcqix: 
            default:            assign(res_t::is_i64, rim.load(ins.im4, 4/*bits*/).read_with_clear<i64>()); break;
            }
        }

        void rdmem(voidp mem, u64 address, uxx bytes){
            // 平坦模式
            // TODO：以后考虑大端模式========================
            for(auto ptr = u08p(mem); bytes-- > 0;){
                ptr[bytes]          = ram[address + bytes];
            }
        }

        void wrmem(voidp mem, u64 address, uxx bytes){
            // 平坦模式
            // TODO：以后考虑大端模式========================
            for(uxx i = 0; i < bytes; i++){
                ram[i + address]    = u08p(mem)[i];
            }
        }

        void asm_ldxx(){
            auto i                  = inc::cast<ldx_t>(ins);
            auto has_sign           = false;
            auto bytes              = ins.opc <= cmd_t::ldqx ? 
                1 << i.scale/*m08/m16/m32/m64*/ : 
                4 << i.scale/*mf32/mf64*/;
            auto address            = i.with_rt ? 
                regs[ins.opb].ru64 + regs[ins.opt].ru64 : 
                regs[ins.opb].ru64;

            // 先设置类型
            // 浮点类型
            if (ins.opc >= cmd_t::lds){
                mode[ins.opa]       = ins.opc & 1 ? res_t::is_f64 : res_t::is_f32;
            }
            // 整数类型
            else{
                has_sign            = i.sign_extern != 0;
                mode[ins.opa]       = has_sign ? res_t::is_i64 : res_t::is_u64;
            }

            regs[ins.opa].ru64      = 0;
            rdmem(& regs[ins.opa]/*des*/, address, bytes);

            if (has_sign){
                regs[ins.opa].ri64  = sign_extern(regs[ins.opa], i.scale);
            }
        }

        void asm_stkxx(){
            auto ins           = inc::cast<rwss_t>(the.ins);
            auto imm           = rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>();
            auto addr          = ss.address - imm * sizeof(reg_t);
            wrmem(& regs[ins.opa], addr, sizeof(reg_t));

            if (ins.side_effect){
                ss.address     = addr;
            }
        }

        void asm_ldkxx(){
            auto ins           = inc::cast<rwss_t>(the.ins);
            auto imm           = rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>();
            auto addr          = ss.address + imm * sizeof(reg_t);

            // 先设置寄存器类型
            mode[ins.opa]      = res_t(ins.type);
            rdmem(& regs[ins.opa], addr, sizeof(reg_t));

            if (ins.side_effect){
                ss.address     = addr;
            }
        }

        void asm_stxx(){
            auto i                  = inc::cast<stxx_t>(ins);
            auto bytes              = 1 << i.scale;
            auto address            = i.with_rt ? 
                regs[ins.opb].ru64 + regs[ins.opt].ru64 : 
                regs[ins.opb].ru64;
            wrmem(& regs[ins.opa], address, bytes);
        }

        template<class opr>
        void f4(opr && invoke){
            #define a   regs[ins.opa]
            #define b   regs[ins.opb]
            #define t   regs[ins.opt]

            switch(f4_t(ins.opc & 0x3)){
            case f4_t::f4aab: mode[ins.opa] = mode[ins.opa]; invoke(a, a, b); break;
            case f4_t::f4abt: mode[ins.opa] = mode[ins.opb]; invoke(a, b, t); break;
            case f4_t::f4tab: mode[ins.opt] = mode[ins.opa]; invoke(t, a, b); break;
            case f4_t::f4tai: mode[ins.opt] = mode[ins.opa]; invoke(t, a, rim.load(ins.opb, 4/*bits*/).read_with_clear<i64>()); break;
            }

            #undef a
            #undef b
            #undef t
        }

        #define xgen(name,...)                                                                      \
        void asm_ ## name(){                                                                        \
            f4([&](reg_t & a, reg_t b, reg_t c){                                                    \
                a.ru64 = __VA_ARGS__;                                                               \
                sta.zf = a.ru64 == 0;                                                               \
            });                                                                                     \
        }

        xgen(band ,  (b.ru64 & c.ru64))
        xgen(bor  ,  (b.ru64 | c.ru64))
        xgen(bxor ,  (b.ru64 ^ c.ru64))
        xgen(bnand, ~(b.ru64 & c.ru64))

        #undef  xgen

        template<class reg, class opr>
        void f8(res_t mode, reg & a, reg b, reg c, opr && invoke){
            switch(mode){
            case is_f32: invoke(a.rf32, b.rf32, c.rf32); break;
            case is_f64: invoke(a.rf64, b.rf64, c.rf64); break;
            case is_u64: invoke(a.ru64, b.ru64, c.ru64); break;
            case is_i64: invoke(a.ri64, b.ri64, c.ri64); break;
            }
        }

        template<class reg, class opr>
        void f8x(res_t mode, reg & a, reg b, reg c, opr && invoke){
            if (mode == is_i64){
                invoke(a.ri64, b.ri64, c.ri64);
            }
            else{
                invoke(a.ru64, b.ru64, c.ru64);
            }
        }

        template<class opr, class subop>
        void f8(opr && invoke, subop && sub_f8){
            auto    role_type   = is_f32;
            auto    i           = reg_t{};
            auto    m           = f8_t(ins.opc & 0x7);

            switch(m){
            case f8abt:  role_type = mode[ins.opb]; break;
            case f8atb:  role_type = mode[ins.opt]; break;
            // case f8aab:  case f8tab:  case f8aai : case f8aia : case f8tai : case f8tia : 
            default:     role_type = mode[ins.opa]; rim.load(ins.im4, 4/*bit*/); break;
            }

            switch(role_type){
            case is_f32: i.rf32 = rim.read_with_clear<f32>(); break;
            case is_f64: i.rf64 = rim.read_with_clear<f64>(); break;
            case is_u64: i.ru64 = rim.read_with_clear<u64>(); break;
            // case is_i64: 
            default:     i.ri64 = rim.read_with_clear<i64>(); break;
            }

            // 如果 a 是临时寄存器，则 regs[ins.opa] 代表的寄存器依赖于 mode[ins.opa] 的值
            #define a       regs[ins.opa]
            #define b       regs[ins.opb]
            #define t       regs[ins.opt]

            switch(m){
            case f8abt: mode[ins.opa] = role_type; sub_f8(role_type, a, b, t, invoke); break;
            case f8atb: mode[ins.opa] = role_type; sub_f8(role_type, a, t, b, invoke); break;
            case f8aab: mode[ins.opa] = role_type; sub_f8(role_type, a, a, b, invoke); break;
            case f8tab: mode[ins.opt] = role_type; sub_f8(role_type, t, a, b, invoke); break;
            case f8aai: mode[ins.opa] = role_type; sub_f8(role_type, a, a, i, invoke); break;
            case f8aia: mode[ins.opa] = role_type; sub_f8(role_type, a, i, a, invoke); break;
            case f8tai: mode[ins.opt] = role_type; sub_f8(role_type, t, a, i, invoke); break;
            // case f8tia: 
            default:    mode[ins.opt] = role_type; sub_f8(role_type, t, i, a, invoke); break;
            }

            #undef  a
            #undef  b
            #undef  t
        }

        template<class opr>
        void f8(opr const & invoke){
            f8(invoke, [this](auto mode, auto & a, auto b, auto c, auto && invoke){
                f8(mode, a, b, c, invoke);
            });
        }

        template<class opr>
        void f8x(opr const & invoke){
            f8(invoke, [this](auto mode, auto & a, auto b, auto c, auto && invoke){
                f8x(mode, a, b, c, invoke);
            });
        }

        template<class type>
        void add(type & a, type b, type c){
            u128 m      = inc::add(u64(b), u64(c));
            a           = type(m.low);

            if constexpr (inc::is_signed<type>){
                auto x0 = u64(b & c) >> 63;
                auto x1 = u64(b | c) >> 63;

                // 结果值的符号位
                auto x2 = u64(a) >> 63;

                // 下溢：同时为负数时，结果符号位变化
                sta.of  = (x0 != 0 and x2 == 0);

                // 上溢：同时为正数，结果符号位变化
                sta.cf  = (x1 == 0 and x2 == 1);
            }
            else{
                sta.cf  = (m.high);
                sta.of  = (0);
            }
        }

        void asm_add(){
            f8([&](auto & a, auto b, auto c){
                if (inc::is_float<decltype(a)>){
                    a = b + c;
                }
                else{
                    add(a, b, c);
                }
            });
        }

        void asm_sub(){
            f8([&](auto & a, auto b, auto c){
                if (inc::is_float<decltype(a)>){
                    a = b - c;
                }
                else{
                    add(a, b, 0 - c);
                }

                sta.gt = b > c;
                sta.eq = b == c;
            });
        }

        void asm_mul(){
            f8([&](auto & a, auto b, auto c){
                using u_t = decltype(b);

                if constexpr (inc::is_float<u_t>){
                    a = b * c;
                }
                else{
                    u128 m; 

                    // 符号位不相等结果为负数，需要符号位扩展
                    if (inc::is_signed<u_t> and (u64(b ^ c) >> 63)){
                        m       = inc::mul(u64(b), u64(c)); 
                        m.high |= u64(-1) << inc::index_of_last_set(m.high);
                        sta.cf  = 0;
                        sta.of  = ~m.high != 0;
                    }
                    else {
                        if (inc::is_signed<u_t> and b < 0){
                            m   = inc::mul(u64(-b), u64(-c));
                        }
                        else{
                            m   = inc::mul(u64(b), u64(c));
                        }

                        sta.cf  = m.high != 0;
                        sta.of  = 0;
                    }

                    if (a = m.low; sta.pmulh != no_predetermined){
                        regs[sta.pmulh] = m.high;
                        sta.pmulh       = no_predetermined;
                    }
                }
            });
        }

        void asm_div(){
            f8([&](auto & a, auto b, auto c){
                using u_t = decltype(b);
                if (c == 0){
                    if (b > 0){
                        a           = inc::max_value_of<u_t>;
                    }
                    else if (b < 0){
                        a           = inc::min_value_of<u_t>;
                    }
                    else{
                        a           = 1;
                    }

                    sta.cf          = b > 0;
                    sta.of          = b < 0;
                    sta.pmod        = no_predetermined;
                    return;
                }

                a                   = b / c;

                if constexpr (inc::is_integer<u_t>){
                    if (sta.pmod != no_predetermined){
                        regs[sta.pmod]
                                    = b % c;
                    }
                }

                sta.pmod            = no_predetermined;
            });
        }

        void asm_shr(){
            f8x([&](auto & a, auto b, auto c){
                u64  overflow_part;
                u128 m              = inc::shift_right(u64(b), u64(c));
                overflow_part       = m.low;
                a                   = m.high;
                sta.cf              = overflow_part >> 63;

                if (a != 0){
                    a              |= u64(-1) << inc::index_of_last_set(a);
                }

                sta.zf              = a == 0;

                if (sta.psfto != no_predetermined){
                    regs[sta.psfto] = overflow_part;
                    sta.psfto       = no_predetermined;
                }
            });
        }

        void asm_shl(){
            f8x([&](auto & a, auto b, auto c){
                u64  overflow_part;
                u128 m              = inc::shift_left(u64(b), u64(c));
                overflow_part       = m.high;
                a                   = m.low;
                sta.cf              = overflow_part & 1;
                sta.zf              = a == 0;

                if (sta.psfto != no_predetermined){
                    regs[sta.psfto] = overflow_part;
                    sta.psfto       = no_predetermined;
                }
            });
        }

        void asm_cmp(){
            reg_t  ra;
            reg_t  rb;
            res_t  ma;
            res_t  mb;

            if (c4_t(ins.opc) == c4ab){
                ra = regs[ins.opa];
                ma = mode[ins.opa];
                rb = regs[ins.opb];
                mb = mode[ins.opb];
            }
            else{
                ra = regs[ins.opa];
                mb = 
                ma = mode[ins.opa];
                rim.load(ins.im4, 4/*bits*/);

                switch(ma){
                case is_f32: rb.rf32 = rim.read_with_clear<f32>(); break;
                case is_f64: rb.rf64 = rim.read_with_clear<f64>(); break;
                case is_u64: rb.ru64 = rim.read_with_clear<u64>(); break;
                // case is_i64: 
                default:     rb.ri64 = rim.read_with_clear<i64>(); break;
                }

                if (c4_t(ins.opc) == c4ia){
                    inc::swap(xref ra, xref rb);
                }
            }

            #define xgen(value)                                                                                 \
            switch(mb){                                                                                         \
            case is_f32: sta.eq = f80(value) == f80(rb.rf32); sta.gt = f80(value) > f80(rb.rf32); break;        \
            case is_f64: sta.eq = f80(value) == f80(rb.rf64); sta.gt = f80(value) > f80(rb.rf64); break;        \
            case is_u64: sta.eq = f80(value) == f80(rb.ru64); sta.gt = f80(value) > f80(rb.ru64); break;        \
            default:     sta.eq = f80(value) == f80(rb.ri64); sta.gt = f80(value) > f80(rb.ri64); break;        \
            }

            switch(ma){
            case is_f32: xgen(ra.rf32) break;
            case is_f64: xgen(ra.rf64) break;
            case is_u64: xgen(ra.ru64) break;
            // case is_i64: 
            default:     xgen(ra.ri64) break;
            }

            #undef  xgen
        }
    };
}

/* 笔记：
 * 当进行跨段调用时，需要进行冗余的寄存器类型设置以避免计算错误，因为外来的参数需要默认当作不可信的（防卫式编程）。
 */

#endif

xexport_space(mixc::extern_isa_cpu::origin)
