#ifndef xpack_extern_isa_cpu
#define xpack_extern_isa_cpu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_isa_cpu::origin
#include"define/base_type.hpp"
#include"macro/xdefer.hpp"
#include"math/const.hpp"
#include"memop/cast.hpp"
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
        // u08/i08 -> mu08
        // u16/i16 -> mu16
        // u32/i32 -> mu32
        // u64/i64 -> mu64
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
        f8abti,
        f8atbi,
        f8aabi,
        f8tabi,
        f8aai,
        f8aia,
        f8tai,
        f8tia,
    };

    struct cpu_t{
        using the_t = cpu_t;
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
            u08     opc;

            union{
                struct{
                    u08 opa : 4;
                    u08 opb : 4;
                };
                u08 im;
            };

            enum{ opt = 15 };
        };

        typedef enum register_state_t{
            is_f32,
            is_f64,
            is_u64,
            is_i64,
        } res_t;

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
            u32     gt                  : 1;

            // 等于
            u32     eq                  : 1;

            // 零标志
            u32     zf                  : 1;

            // 上溢/进位/借位
            u32     cf                  : 1;

            // 下溢
            u32     of                  : 1;

            // 预设(predetermined)
            // 指定余数存放的寄存器
            u32     pmod                : 5;

            // 指定乘法高位积存放的寄存器
            u32     pmulh               : 5;

            // 指定移位溢出位存放的寄存器
            u32     psfto               : 5;
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

        enum{
            general_purpose_register_count  = 0x10,
            no_predetermined                = 0x10,
        };

        imm_t   rim;                                    // 立即数寄存器
        ins_t   ins;                                    // 指令寄存器
        reg_t   regs[general_purpose_register_count];
        reg_t   rs;                                     // 临时 f32 寄存器
        reg_t   rf;                                     // 临时 f64 寄存器
        reg_t & rt      = regs[ins_t::opt];             // 临时 r64 寄存器
        sta_t   sta;                                    // 状态寄存器
        seg_t   pc;                                     // 程序计数器
        seg_t   cs;                                     // 调用栈寄存器
        seg_t   ss;                                     // 堆栈寄存器
        rtg_t & mode    = sta.reg_type;

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

            if (cmode == c4ab){
                b   = unified_reg(ib);
            }
            else{
                rim.load(ib, 4/*bits*/);
                b   = unified_imm(mode[ia]);
            }

            sta.eq  = a == b;
            sta.gt  = cmode == c4ia ? a < b : a > b;
        }

        struct cifxx_t{
            u08 opc     : 4;
            u08 mode    : 2;
            u08 bank    : 2;
            u08 opa     : 2;
            u08 opb     : 2;
            u08 im4     : 4;
        };

        void asm_cifxx(){
            auto ins = inc::cast<cifxx_t>(the.ins);
            auto i   = rim.load(ins.im4, 4/*bits*/);
            cmp(c4_t::c4ab, ins.bank << 2 | ins.opa, ins.bank << 2 | ins.opb);

            switch(cmd_t(ins.mode | cifeq)){
            case cifeq: ifxx(    sta.eq);               break;
            case cifne: ifxx(not sta.eq);               break;
            case cifle: ifxx(    sta.eq or not sta.gt); break;
            case ciflt: ifxx(not sta.eq or not sta.gt); break;
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
            switch(rim.load(ins.im, 8/*bits*/); cmd_t(ins.opc | cmd_t::ifeq)){
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
            case cmd_t::jmp : ifxx(false/*force*/);           break;
            }
        }

        void asm_ret(){

        }

        void jalx(seg_t address){
            
        }

        void asm_imm(){
            auto im = inc::cast<u16>(ins) & 0xfff;
            rim.load(im, 12/*bits*/);
        }

        void asm_mov(){
            switch(cmd_t(ins.opc)){
            case cmd_t::movqb : mode[ins.opa] = res_t::is_u64; regs[ins.opa].ru64 = regs[ins.opb].ru08; break;
            case cmd_t::movqbx: mode[ins.opa] = res_t::is_i64; regs[ins.opa].ri64 = regs[ins.opb].ri08; break;
            case cmd_t::movqw : mode[ins.opa] = res_t::is_u64; regs[ins.opa].ru64 = regs[ins.opb].ru16; break;
            case cmd_t::movqwx: mode[ins.opa] = res_t::is_i64; regs[ins.opa].ri64 = regs[ins.opb].ri16; break;
            case cmd_t::movqd : mode[ins.opa] = res_t::is_u64; regs[ins.opa].ru64 = regs[ins.opb].ru32; break;
            case cmd_t::movqdx: mode[ins.opa] = res_t::is_i64; regs[ins.opa].ri64 = regs[ins.opb].ri32; break;
            case cmd_t::movqq : mode[ins.opa] = res_t::is_u64; regs[ins.opa].ru64 = regs[ins.opb].ru64; break;
            case cmd_t::movqqx: mode[ins.opa] = res_t::is_i64; regs[ins.opa].ri64 = regs[ins.opb].ri64; break;

            case cmd_t::movss : mode[ins.opa] = res_t::is_f32; regs[ins.opa].rf32 = regs[ins.opb].rf32; break;
            case cmd_t::movsf : mode[ins.opa] = res_t::is_f32; regs[ins.opa].rf32 = regs[ins.opb].rf64; break;
            case cmd_t::movsq : mode[ins.opa] = res_t::is_f32; regs[ins.opa].rf32 = regs[ins.opb].ru64; break;
            case cmd_t::movsqx: mode[ins.opa] = res_t::is_f32; regs[ins.opa].rf32 = regs[ins.opb].ri64; break;
            case cmd_t::movfs : mode[ins.opa] = res_t::is_f64; regs[ins.opa].rf64 = regs[ins.opb].rf32; break;
            case cmd_t::movff : mode[ins.opa] = res_t::is_f64; regs[ins.opa].rf64 = regs[ins.opb].rf64; break;
            case cmd_t::movfq : mode[ins.opa] = res_t::is_f64; regs[ins.opa].rf64 = regs[ins.opb].ru64; break;
            case cmd_t::movfqx: mode[ins.opa] = res_t::is_f64; regs[ins.opa].rf64 = regs[ins.opb].ri64; break;

            case cmd_t::movqs : mode[ins.opa] = res_t::is_u64; regs[ins.opa].ru64 = regs[ins.opb].rf32; break;
            case cmd_t::movqsx: mode[ins.opa] = res_t::is_i64; regs[ins.opa].ri64 = regs[ins.opb].ru64; break;
            case cmd_t::movqf : mode[ins.opa] = res_t::is_u64; regs[ins.opa].ru64 = regs[ins.opb].rf64; break;
            case cmd_t::movqfx: mode[ins.opa] = res_t::is_i64; regs[ins.opa].ri64 = regs[ins.opb].ri64; break;
            case cmd_t::movsi : mode[ins.opa] = res_t::is_f32; regs[ins.opa].rf32 = (f32)rim.load(ins.opb, 4/*bits*/).read_with_clear<u64>(); break;
            case cmd_t::movsix: mode[ins.opa] = res_t::is_f32; regs[ins.opa].rf32 = (f32)rim.load(ins.opb, 4/*bits*/).read_with_clear<i64>(); break;
            case cmd_t::movfi : mode[ins.opa] = res_t::is_f64; regs[ins.opa].rf64 = (f64)rim.load(ins.opb, 4/*bits*/).read_with_clear<u64>(); break;
            case cmd_t::movfix: mode[ins.opa] = res_t::is_f64; regs[ins.opa].rf64 = (f64)rim.load(ins.opb, 4/*bits*/).read_with_clear<i64>(); break;
            }
        }

        struct bdc_t{
            u08         : 6;
            u08   bank  : 2;
            u08   bmp   : 4;
            u08   opb   : 4;
        };

        void asm_bdc(){
            auto i      = inc::cast<bdc_t>(ins);
            auto assign = [&](res_t m, auto v){
                auto p_v = & regs[i.bank << 2];

                for(uxx mask = i.bmp, idx = 0; mask != 0; mask >>= 1, idx++){
                    if (0 == (mask & 1)){
                        continue;
                    }

                    p_v [idx]               = v;
                    mode[i.bank << 2 | idx] = m;

                    if (m == res_t::is_f32){
                        p_v[idx].rh32 = 0;
                    }
                }
            };

            switch(cmd_t(ins.opc)){
            case cmd_t::bdcss : assign(res_t::is_f32, regs[i.opb]);                                       break;
            case cmd_t::bdcsi : assign(res_t::is_f32, rim.load(i.opb, 4/*bits*/).read_with_clear<f32>()); break;
            case cmd_t::bdcff : assign(res_t::is_f64, regs[ins.opb]);                                     break;
            case cmd_t::bdcfi : assign(res_t::is_f64, rim.load(i.opb, 4/*bits*/).read_with_clear<f64>()); break;
            case cmd_t::bdcqq : assign(res_t::is_u64, regs[ins.opb]);                                     break;
            case cmd_t::bdcqi : assign(res_t::is_u64, rim.load(i.opb, 4/*bits*/).read_with_clear<u64>()); break;
            case cmd_t::bdcqqx: assign(res_t::is_i64, regs[ins.opb]);                                     break;
            case cmd_t::bdcqix: assign(res_t::is_i64, rim.load(i.opb, 4/*bits*/).read_with_clear<i64>()); break;
            }
        }

        struct ldx_t{
            u08                : 4;
            u08    scale       : 2;
            u08    sign_extern : 1;
            u08    with_rt     : 1;
        };

        void rdmem(reg_t * mem, u64 address, uxx bytes){

        }

        void wrmem(reg_t * mem, u64 address, uxx bytes){

        }

        void asm_ldxx(){
            auto i                  = inc::cast<ldx_t>(ins);
            auto bytes              = ins.opc > cmd_t::ldqx ? (ins.opc - cmd_t::ldqx) * 4 : 1 << i.scale;
            regs[ins.opa].ru64      = 0;
            rdmem(& regs[ins.opa]/*des*/, i.with_rt ? regs[ins.opb].ru64 + rt.ru64 : regs[ins.opb].ru64/*source*/, bytes);

            if (ins.opc > cmd_t::ldqx){
                mode[ins.opa]       = ins.opc & 1 ? res_t::is_f64 : res_t::is_f32;
            }
            else if (i.sign_extern){
                mode[ins.opa]       = res_t::is_i64;
                regs[ins.opa].ri64  = sign_extern(regs[ins.opa], i.scale);
            }
            else{
                mode[ins.opa]       = res_t::is_u64;
            }
        }

        struct rwss_t{
            u08                 : 5;
            u08     type        : 2;
            u08     side_effect : 1;
            u08     opa         : 4;
            u08     im4         : 4;
        };

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
            regs[ins.opa].ru64 = 0;
            mode[ins.opa]      = res_t(ins.type);
            rdmem(& regs[ins.opa], addr, sizeof(reg_t));

            if (ins.side_effect){
                ss.address     = addr;
            }
        }

        struct stxx_t{
            u08                 : 5;
            u08     scale       : 2;
            u08     with_rt     : 1;
        };

        void asm_stxx(){
            auto i                  = inc::cast<stxx_t>(ins);
            auto bytes              = 1 << i.scale;
            wrmem(& regs[ins.opa]/*source*/, i.with_rt ? regs[ins.opb].ru64 + rt.ru64 : regs[ins.opb].ru64/*target*/, bytes);
        }

        template<class opr>
        void f4(opr const & invoke){
            switch(f4_t(ins.opc & 0x3)){
            case f4_t::f4aab: invoke(mode[ins.opa], regs[ins.opa], regs[ins.opa], regs[ins.opb]);                                       break;
            case f4_t::f4abt: invoke(mode[ins.opb], regs[ins.opa], regs[ins.opb], regs[ins.opt]);                                       break;
            case f4_t::f4tab: invoke(mode[ins.opa], regs[ins.opt], regs[ins.opa], regs[ins.opb]);                                       break;
            case f4_t::f4tai: invoke(mode[ins.opa], regs[ins.opt], regs[ins.opa], rim.load(ins.opb, 4/*bits*/).read_with_clear<i64>()); break;
            }
        }

        void asm_band(){
            f4([&](res_t m, reg_t & a, reg_t b, reg_t c){
                a.ru64 = b.ru64 & c.ru64;
                sta.zf = a.ru64 == 0;
            });
        }

        void asm_bor(){
            f4([&](res_t m, reg_t & a, reg_t b, reg_t c){
                a.ru64 = b.ru64 | c.ru64;
                sta.zf = a.ru64 == 0;
            });
        }

        void asm_bxor(){
            f4([&](res_t m, reg_t & a, reg_t b, reg_t c){
                a.ru64 = b.ru64 ^ c.ru64;
                sta.zf = a.ru64 == 0;
            });
        }

        void asm_bnand(){
            f4([&](res_t m, reg_t & a, reg_t b, reg_t c){
                a.ru64 = ~(b.ru64 & c.ru64);
                sta.zf = a.ru64 == 0;
            });
        }

        #define xgen(rt,type)                                                                       \
        switch(m){                                                                                  \
        case f8abti: invoke(ra.r ## type , rb.r ## type , rt          ); return;                    \
        case f8atbi: invoke(ra.r ## type , rt           , rb.r ## type); return;                    \
        case f8aabi: invoke(ra.r ## type , ra.r ## type , rb.r ## type); return;                    \
        case f8tabi: invoke(rt           , ra.r ## type , rb.r ## type); return;                    \
        }                                                                                           \
                                                                                                    \
        rim.load(ins.opb, 4/*bits*/);                                                               \
                                                                                                    \
        switch(m){                                                                                  \
        case f8aai : invoke(ra.r ## type , ra.r ## type, rim.read_with_clear<type>()); return;      \
        case f8aia : invoke(ra.r ## type , rim.read_with_clear<type>(), ra.r ## type); return;      \
        case f8tai : invoke(rt           , ra.r ## type, rim.read_with_clear<type>()); return;      \
        case f8tia : invoke(rt           , rim.read_with_clear<type>(), ra.r ## type); return;      \
        }

        template<class opr>
        void f8(opr const & invoke){
            auto & ra = regs[ins.opa];
            auto & rb = regs[ins.opb];
            auto   m  = f8_t(ins.opc & 0x7);

            switch(res_t(mode[ins.opa])){
            case is_f32: xgen(rs.rf32, f32) break;
            case is_f64: xgen(rf.rf64, f64) break;
            case is_u64: xgen(rt.ru64, u64) break;
            case is_i64: xgen(rt.ri64, i64) break;
            }
        }

        template<class opr>
        void f8x(opr const & invoke){
            auto & ra = regs[ins.opa];
            auto & rb = regs[ins.opb];
            auto   m  = f8_t(ins.opc & 0x7);

            if (res_t(mode[ins.opa]) == is_i64){
                xgen(rt.ri64, i64)
            }
            else{
                xgen(rt.ru64, u64)
            }
        }

        #undef xgen

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

        void sub(){
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
                using ut = decltype(b);

                if constexpr (inc::is_float<ut>){
                    a = b * c;
                }
                else{
                    u128 m; 

                    // 符号位不相等结果为负数，需要符号位扩展
                    if (inc::is_signed<ut> and (u64(b ^ c) >> 63)){
                        m       = inc::mul(u64(b), u64(c)); 
                        m.high |= u64(-1) << inc::index_of_last_set(m.high);
                        sta.cf  = 0;
                        sta.of  = ~m.high != 0;
                    }
                    else {
                        if (inc::is_signed<ut> and b < 0){
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
                using ut = decltype(b);
                if (sta.of = c == 0; sta.of){
                    if (b > 0){
                        a   = inc::max_value_of<ut>;
                    }
                    else if (b < 0){
                        a   = inc::min_value_of<ut>;
                    }
                    else{
                        a   = 1;
                    }
                    return;
                }

                if constexpr (inc::is_integer<ut>){
                    if (sta.pmod != no_predetermined){
                        regs[sta.pmod]  = b % c;
                        sta.pmod        = no_predetermined;
                    }
                }
                a = b / c;
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
            res_t  sa;
            res_t  sb;

            if (c4_t(ins.opc) == c4ab){
                ra = regs[ins.opa];
                sa = mode[ins.opa];
                rb = regs[ins.opb];
                sb = mode[ins.opb];
            }
            else{
                ra = regs[ins.opa];
                sb = 
                sa = mode[ins.opa];
                rim.load(ins.opb, 4/*bits*/);

                switch(c4_t(ins.opc)){
                case is_f32: rb.rf32 = rim.read_with_clear<f32>(); break;
                case is_f64: rb.rf64 = rim.read_with_clear<f64>(); break;
                case is_u64: rb.ru64 = rim.read_with_clear<u64>(); break;
                case is_i64: rb.ri64 = rim.read_with_clear<i64>(); break;
                }

                if (c4_t(ins.opc) == c4ia){
                    inc::swap(xref ra, xref rb);
                }
            }

            #define xgen(type)                                                                              \
            switch(sb){                                                                                     \
            case is_f32: sta.eq = f80(type) == f80(rb.rf32); sta.gt = f80(type) > f80(rb.rf32); break;      \
            case is_f64: sta.eq = f80(type) == f80(rb.rf64); sta.gt = f80(type) > f80(rb.rf64); break;      \
            case is_u64: sta.eq = f80(type) == f80(rb.ru64); sta.gt = f80(type) > f80(rb.ru64); break;      \
            case is_i64: sta.eq = f80(type) == f80(rb.ri64); sta.gt = f80(type) > f80(rb.ri64); break;      \
            }

            switch(sa){
            case is_f32: xgen(ra.rf32) break;
            case is_f64: xgen(ra.rf64) break;
            case is_u64: xgen(ra.ru64) break;
            case is_i64: xgen(ra.ri64) break;
            }

            #undef  xgen
        }
    };
}

/*
 * 
 * bp + 16  arg2
 * bp + 08  arg1
 * bp + 00  arg0
 * 
 * 0x00 <- 0xff10
 * 0x04 <- 0xff0c
 * 
 * 
 * 调用框架：
 * 
 *      # 保留 n 个单位的内存用于保存返回参数
 *      keep n
 *
 *      ...
 *      push mem_arg8
 *      push mem_arg7
 *      push mem_arg6
 *      rf   reg_arg5
 *      re   reg_arg4
 *      rd   reg_arg3
 *      rc   reg_arg2
 *      rb   reg_arg1
 *      ra   reg_arg0
 *      r9   reg_ret1
 *      r8   reg_ret0
 * 
 *      jal  foo -> r0, r1
 * 
 *      jal  8
 *      ret  8
 * 
 */

#endif
