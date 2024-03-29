/* 本模块主要用于模拟 Mix-C ISA cpu 行为，有些逻辑为了方便使用了低效的实现
 * 
 * 注意：
 * 不接受 sta.predetermined 的副作用在不同代码段中扩散，可能会构成安全漏洞
 * jump 类操作仅允许在所属的段中跳转，因为它不复位 sta 中的状态
 * 
 * 当进行跨段调用时，需要进行冗余的寄存器类型设置以避免计算错误，因为外来的参数需要默认当作不可信的。
 */
#ifndef xpack_extern_isa_vcpu
#define xpack_extern_isa_vcpu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_isa_vcpu::origin::inc
#include"define/base_type.hpp"
#include"configure/platform.hpp"
#include"instruction/add.hpp"
#include"instruction/index_of_last_set.hpp"
#include"instruction/mul.hpp"
#include"instruction/shift_left.hpp"
#include"instruction/shift_right.hpp"
#include"instruction/sub.hpp"
#include"macro/xexport.hpp"
#include"macro/xdefer.hpp"
#include"macro/xref.hpp"
#include"math/const.hpp"
#include"memop/cast.hpp"
#include"memop/copy.hpp"
#include"memop/swap.hpp"
#include"meta/is_float.hpp"
#include"meta/is_integer.hpp"
#include"meta/is_signed.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_isa_vcpu::origin{
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
        ifuf            ,
        ifnu            ,
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

        // 内存读取
        // f32 <- mf32
        // f64 <- mf64
        lds             = ldqx   + 2,
        ldf             = lds    + 2,

        // 内存读取（带 null 值校验）
        lddz            = ldf    + 2,
        ldqz            = lddz   + 2,

        // 写入内存
        // u08/i08      -> mu08
        // u16/i16      -> mu16
        // u32/i32/f32  -> mu32
        // u64/i64/f64  -> mu64
        stb             = ldqz   + 2,
        stw             = stb    + 2,
        std             = stw    + 2,
        stq             = std    + 2,

        // 基础逻辑运算
        band            = stq    + 2,
        bor             = band   + 4,
        bxor            = bor    + 4,
        bnand           = bxor   + 4,

        // 算术运算
        add             = bnand  + 4,
        sub             = add    + 8,
        mul             = sub    + 8,
        div             = mul    + 8,

        // 有符号数右移和除以 2 的结果一致
        // -1 >> 1 会变成 0， -2 >> 2 也会变成 0
        // 这点和其他 isa 不同
        shr             = div    + 8,
        shl             = shr    + 8,

        // 比较
        cmp             = shl    + 8,

        // 位操作类
        bop             = cmp    + 4,

        // 最小最大值
        miax            = bop    + 4,

        // 交换两个值
        swap            ,

        // 内部互斥锁
        lock            ,

        // 标记类
        token           ,

        // 立即数加载
        imm             = 256 - 16,
    };

    enum token_t{
        // 调试中断标记
        brk,
    };

    enum rduxr_t{
        rdsta           , // 读取状态寄存器
        rdmod           , // 预定除法余数
        rdproh          , // 预定乘法高位
        rdsfto          , // 预定移位溢出位组
        rdtms           , // 读取时间戳
    };

    enum wruxr_t{
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
    };

    struct cifxx_t{
        u08 opc         : 4;
        u08 mode        : 2;
        u08 bank        : 2;

        static_assert(general_purpose_register_count == 0x10);

        enum{ get_bank_by_shift = 2, };

        u08 opa         : 2;
        u08 opb         : 2;
        u08 im4         : 4;
    };

    struct jalx_t{
        u08             : 8;
        u08             : 2;
        u08 ps          : 1; // predetermined save
        u08 sf          : 1; // save flag field
        u08 im4_opa     : 4;
    };

    struct bdc_t{
        u08             : 6;
        u08 bank        : 2;
        u08 bmp         : 4;
        u08 opb         : 4;
    };

    struct ldx_t{
        u08             : 4;
        u08 scale       : 2;
        u08 sign_extern : 1;
        u08 with_rt     : 1;
    };

    struct rwss_t{
        u08             : 5;
        u08 type        : 2;
        u08 side_effect : 1;
        u08 opa         : 4;
        u08 im4         : 4;
    };

    struct stxx_t{
        u08             : 5;
        u08 scale       : 2;
        u08 with_rt     : 1;
    };

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
                this->clear();
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

        void clear(){
            imm         = 0;
            total_bits  = 0;
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

    template<class bits_t, class field_t, uxx bits_v>
    struct bits_group{
    private:
        bits_t bits;

        enum{
            mask = (1 << bits_v) - 1
        };

        struct bits_delegate{
            bits_delegate(bits_t * bits, uxx offset) : 
                bits(bits), offset(offset){
            }

            operator field_t(){
                return field_t(
                    (*bits >> offset) & mask
                );
            }

            field_t operator=(field_t value){
                *bits &= ~(mask  << offset);
                *bits |=  (value << offset);
                return value;
            }
        private:
            bits_t *    bits;
            uxx         offset;
        };

    public:
        bits_group(bits_t value = 0) : bits(value){}

        bits_delegate operator [](uxx index){
            return bits_delegate(xref(bits), index * bits_v);
        }
    };

    using reg_type_group    = bits_group<u32, register_state_t, 2/*bit per item*/>;
    using reg_index_group   = bits_group<u64, uxx, 4/*bit per item*/>;
    using rtg_t             = reg_type_group;
    using rig_t             = reg_index_group;

    typedef struct context_group_t{
        enum{ count_per_group = 16, mask = count_per_group - 1 };

        context_group_t(){}
        context_group_t(u32 prev, rtg_t type, u64 data[count_per_group]) :
            prev(prev), type(type){
            inc::copy(this->data, data, count_per_group);
        }

        u32     prev;
        rtg_t   type;                   // 每一个存放的元素是哪一种类型
        u64     data[count_per_group];  // 每组 16 个元素
    } ctx_t;

    struct sta_t{
        union{
            struct{
                // 大于
                u16     gt  : 1;

                // 等于
                u16     eq  : 1;

                // 零标志
                u16     zf  : 1;

                // 上溢/进位/借位
                u16     cf  : 1;

                // 下溢 underflow
                u16     uf  : 1;

                // 保留
                u16         : 11;
            };
            u16 flag        = 0;
        };
    private:
        // 保留
        u16     r0          = 0;
    public:
        enum{
            reset_predetermined =
                no_predetermined << 10 | 
                no_predetermined << 5  | 
                no_predetermined << 0,

            // 寄存器类型 2bit * 16
            // - 00 f32
            // - 01 f64
            // - 10 u64（默认）
            // - 11 i64
            // 和 opcode 保持一致
            reset_reg_type      = 0xaaaaaaaa,
        };

        union{
            struct{
                // 预设(predetermined)
                // 指定余数存放的寄存器
                // 在指定寄存器获取到余数后该位域被复位为 no_predetermined
                u16     pmod        : 5;

                // 指定乘法高位积存放的寄存器
                // 在指定寄存器获取到高位积后该位域被复位为 no_predetermined
                u16     pmulh       : 5;

                // 指定移位溢出位存放的寄存器
                // 在指定寄存器获取到溢出位后该位域被复位为 no_predetermined
                u16     psfto       : 5;

                // 保留
                u16                 : 1;
            };

            u16 predetermined = reset_predetermined;
        };

        // 当前函数需要保存的通用寄存器上下文(惰性)
        // predetermined save
        u16     ps          = 0;

        // predetermined save mask
        // 每一位分别指示对应的通用寄存器在下一次修改前需要保存
        // 每一级调用可以叠加
        u16     psm         = 0;

        // 寄存器类型 2bit * 16
        // - 00 f32
        // - 01 f64
        // - 10 u64（默认）
        // - 11 i64
        // 和 opcode 保持一致
        rtg_t   reg_type    = reset_reg_type;
    };

    // 段偏式
    union seg_t{
        // 仅用于代码段
        struct{
            u16 over_area   : 1;
        };

        enum : u64{
            mask_code_segment = u64(~0x1),
        };

        // 仅用于代码段
        struct{
            u16 position;
            u16 area;
        };

        // 用于代码段和数据段
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

    struct vcpu_t;

    inline voidp cast(void(vcpu_t::* func)()){
        union {
            voidp          mem;
            void (vcpu_t::* call)();
        } u;

        u.call = func;
        return u.mem;
    }

    inline i64 sign_extern(reg_t val, uxx scale){
        auto idx  = 1ull << ((1ull << scale) * 8);
        auto test = idx >> 1;
        auto mask = idx - 1;

        // zero extern : sign extern
        val.ri64 = val.ri64 & test ? val.ri64 & mask : val.ri64 | ~mask;
        return val.ri64;
    }

    inline i64 zero_extern(reg_t val, uxx scale){
        auto idx  = 1ull << ((1ull << scale) * 8);
        auto mask = idx - 1;

        // zero extern
        val.ri64 = val.ri64 & mask;
        return val.ri64;
    }

    struct vcpu_t{
    public:
        vcpu_t(uxx bytes) : 
            ram(inc::alloc<u08>(inc::memory_size{bytes})){

            #define xgen(start,end,func)                                    \
                for(uxx i = uxx(cmd_t::start); i < uxx(cmd_t::end); i++)    \
                    cmd[i] = cast(& vcpu_t::func);

            xgen(cifeq, ciflt +1, asm_cif)
            xgen(ifeq , jmp   +1, asm_if)
            xgen(ret  , ret   +1, asm_ret)
            xgen(jali , jalr  +1, asm_jal)
            xgen(bdcss, bdcqix+1, asm_bdc)
            xgen(movqb, movfix+1, asm_mov)

            // xgen(rduxr, rduxr +1, )
            // xgen(rdpri, rdpri +1, )
            // xgen(wruxr, wruxr +1, )
            // xgen(wrpri, wrpri +1, )

            xgen(ldb  , ldqx  +1, asm_ld)
            xgen(lds  , ldf   +1, asm_ld)
            xgen(stb  , stq   +1, asm_st)
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
        using the_t     = vcpu_t;

        u08p    ram{};                                  // 内存起始地址
        imm_t   rim{};                                  // 立即数寄存器
        ins_t   ins{};                                  // 指令寄存器
        reg_t   regs[general_purpose_register_count];   // 通用寄存器组
        sta_t   sta{};                                  // 状态寄存器
        seg_t   pc{};                                   // 程序计数器
        seg_t   cs{};                                   // 调用栈寄存器
        seg_t   ss{};                                   // 堆栈寄存器
        seg_t   xs{};                                   // 上下文寄存器
        rtg_t & mode    = sta.reg_type;                 // 寄存器类型
        voidp   cmd[256];                               // 指令集清单

        struct{
            enum { 
                group_count     = 4, 

                count_per_group = ctx_t::count_per_group,

                // 存满
                full            = group_count * ctx_t::count_per_group,

                // 当只剩一组可存时可以刷新最早的组到内存，这样就可以空出一组
                need_store      = full - ctx_t::count_per_group,

                // 当只剩一组可取时可以从内存中加载一组
                need_load       = ctx_t::count_per_group,

                // 获取组号
                mask_group      = group_count - 1,

                // 处于只剩一组可存时
                to_first        = 2,

                // 处于只剩一组可读时
                to_last         = 1,
            };

            // 需要二进制对齐
            static_assert((group_count & (group_count - 1)) == 0);

            seg_t       mem         = not_exist;
            uxx         counter     = 0;

            // 饱和计数器，计数达到 full 时就将较早的一组 data 刷新到内存并让该计数器减半
            uxx         counterx    = 0;

            struct{
                // 16 * 2bit
                rtg_t   type;
                u64     data[ctx_t::count_per_group];
            } group[group_count];
        } ctx[general_purpose_register_count];

        // 注意：需要放在修改寄存器类型、值之前
        void before_modify_register(uxx i_reg){
            if (u16 mask = u16(1 << i_reg); not (sta.psm & mask)){
                return;
            }
            else{
                sta.psm            ^= (mask);
            }

            auto & ctx              = (this->ctx[i_reg]); 
            auto   i                = (ctx.counter & ctx_t::count_per_group) != 0;
            auto   ix               = (i + ctx.to_first/*回绕到最早的一组*/) & ctx.mask_group;
            auto   offset           = (ctx.counter & ctx_t::mask);
            auto & data             = (ctx.group[i].data);
            auto & type             = (ctx.group[i].type);

            data[offset]            = (regs[i_reg].ru64);
            type[offset]            = (mode[i_reg]);
            ctx.counter            += (1);
            ctx.counterx           += (1);

            // 只有当 need_store 时以下字段才是有效的
            auto & old_data         = (ctx.group[ix].data);
            auto & old_type         = (ctx.group[ix].type);

            if (ctx.counterx == ctx.need_store){
                ctx_t new_ctx{ ctx.mem.offset/*前一个块*/, old_type, old_data };
                ctx.mem             = (xs);
                xs.offset          += (sizeof(ctx_t));
                wrmem(& new_ctx, ctx.mem.address, sizeof(ctx_t));
                return;
            }

            // 存满时，让之前存的一组空出来
            if (ctx.counterx == ctx.full){
                ctx.counterx       -= (ctx.count_per_group);
            }
        }

        void after_return(uxx need_recover){
            for(uxx i_reg = 0;; need_recover ^= uxx(1) << i_reg/*复位*/){
                if (i_reg = inc::index_of_last_set(need_recover); i_reg == not_exist){
                    return;
                }

                // 异常情况
                // 代码段可能被修改过，或者读取了错误的 sta.ps 寄存器值
                if (this->ctx[i_reg].counter == 0){
                    continue;
                }

                auto & ctx          = (this->ctx[i_reg]);
                auto   i            = (--ctx.counter & ctx_t::count_per_group) != 0;
                auto   ix           = (i - ctx.to_last) & ctx.mask_group;
                auto   offset       = (ctx.counter & ctx_t::mask);
                auto & data         = (ctx.group[i].data);
                auto & type         = (ctx.group[i].type);

                // 只有当 need_load 时以下字段才是有效的
                auto & old_data     = (ctx.group[ix].data);
                auto & old_type     = (ctx.group[ix].type);

                // 需要预先加载一个组
                if (ctx.counterx = (ctx.counterx - 1);
                    ctx.counterx == ctx.need_load and ctx.counter > ctx.counterx){
                    ctx_t buffer;
                    rdmem(& buffer, ctx.mem.address, sizeof(buffer));
                    inc::copy(old_data, buffer.data, buffer.count_per_group);
                    old_type        = (buffer.type);
                    ctx.mem.offset  = (buffer.prev);
                    ctx.counterx   += (ctx.count_per_group);
                }

                // 恢复寄存器类型和值
                mode[i_reg]         = (type[offset]);
                regs[i_reg]         = (data[offset]);

                // TODO 回收释放的内存===========================================
            }
        }

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
                sta.uf  = (x0 != 0 and x2 == 0);

                // 上溢：同时为正数，结果符号位变化
                sta.cf  = (x1 == 0 and x2 == 1);
            }
            else{
                sta.cf  = (m.high);
                sta.uf  = (0);
            }
        }

        void ifxx(bool contiguous){
            // 当前加载位数的立即数最高位为 1 时会进行符号位扩展
            // 但只加载了 4bit 的 i64 类型不会扩展符号
            // 这样是为了让 cif 使用完整的 4bit 向下跳转
            i64 offset      = rim.read_with_clear<i64>(); 

            // 不满足条件就跳转
            // 向上跳转（回跳）
            // 向下跳转
            // 不允许跨段跳转
            if (not contiguous){
                pc.offset  += u32(offset) * sizeof(ins_t);
            }
        }

        template<class opr>
        void f4(opr && invoke){
            #define a   regs[ins.opa]
            #define b   regs[ins.opb]
            #define t   regs[ins.opt]

            switch(f4_t(ins.opc & 0x3)){
            case f4_t::f4aab: 
                before_modify_register(ins.opa);
                mode[ins.opa] = mode[ins.opa];
                invoke(a, a, b);
                break;
            case f4_t::f4abt: 
                before_modify_register(ins.opa);
                mode[ins.opa] = mode[ins.opb];
                invoke(a, b, t);
                break;
            case f4_t::f4tab:
                before_modify_register(ins.opt);
                mode[ins.opt] = mode[ins.opa];
                invoke(t, a, b);
                break;
            case f4_t::f4tai:
                before_modify_register(ins.opt);
                mode[ins.opt] = mode[ins.opa];
                invoke(t, a, rim.load(ins.im4, 4/*bits*/).read_with_clear<i64>());
                break;
            }

            #undef a
            #undef b
            #undef t
        }

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
            case f8abt:
                before_modify_register(ins.opa);
                mode[ins.opa] = role_type;
                sub_f8(role_type, a, b, t, invoke);
                break;
            case f8atb:
                before_modify_register(ins.opa);
                mode[ins.opa] = role_type;
                sub_f8(role_type, a, t, b, invoke);
                break;
            case f8aab:
                before_modify_register(ins.opa);
                mode[ins.opa] = role_type;
                sub_f8(role_type, a, a, b, invoke);
                break;
            case f8tab:
                before_modify_register(ins.opt);
                mode[ins.opt] = role_type;
                sub_f8(role_type, t, a, b, invoke);
                break;
            case f8aai:
                before_modify_register(ins.opa);
                mode[ins.opa] = role_type;
                sub_f8(role_type, a, a, i, invoke);
                break;
            case f8aia:
                before_modify_register(ins.opa);
                mode[ins.opa] = role_type;
                sub_f8(role_type, a, i, a, invoke);
                break;
            case f8tai:
                before_modify_register(ins.opt);
                mode[ins.opt] = role_type;
                sub_f8(role_type, t, a, i, invoke);
                break;
            // case f8tia: 
            default:
                before_modify_register(ins.opt);
                mode[ins.opt] = role_type;
                sub_f8(role_type, t, i, a, invoke);
                break;
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

        void asm_cif(){
            auto ins = inc::cast<cifxx_t>(the.ins);
            cmp(
                (c4_t::c4ab), 
                (ins.bank << ins.get_bank_by_shift) | ins.opa, 
                (ins.bank << ins.get_bank_by_shift) | ins.opb
            );

            switch(rim.load(ins.im4, 4/*bits*/); cmd_t(ins.mode | cifeq)){
            case cifeq: ifxx(    sta.eq);               break;
            case cifne: ifxx(not sta.eq);               break;
            case cifle: ifxx(    sta.eq or not sta.gt); break;
            // case ciflt: 
            default:    ifxx(not sta.eq or not sta.gt); break;
            }
        }

        void asm_if(){
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
            case cmd_t::ifuf: ifxx(    sta.uf);               break;
            case cmd_t::ifnu: ifxx(not sta.uf);               break;
            // case cmd_t::jmp : 
            default:          ifxx(false/*force*/);           break;
            }
        }

        void asm_ret(){
            // 跨程序段跳转
            // TODO====================================================================================

            // 清除立即数寄存器
            rim.clear();
            cs.address             -= sizeof(pc.position);
            rdmem(& pc.position, cs.address, sizeof(pc.position));

            if (pc.over_area){
                pc.over_area        = 0;
                cs.address         -= sizeof(pc.position);
                rdmem(& pc.area, cs.address, sizeof(pc.position));
            }

            // 读取 jal 指令信息
            auto ins                = jalx_t{};
            rdmem(& ins, pc.address, sizeof(ins_t));

            // 恢复 flag 状态信息
            if (ins.sf){
                cs.address         -= sizeof(sta.flag);
                rdmem(& sta.flag, cs.address, sizeof(sta.flag));
            }

            // 恢复 ps 通用寄存器
            if (ins.ps){
                static_assert(sizeof(ins_t) == sizeof(sta.ps));
                rdmem(& sta.ps, pc.address + sizeof(ins_t), sizeof(ins_t));
                auto changed        = sta.psm & sta.ps; // 只取与 sta.ps 相关的位
                auto need_recover   = changed ^ sta.ps; // 取出改变的位组
                pc.address         += sizeof(sta.ps);   // 忽略下一条指令
                cs.address         -= sizeof(sta.ps);
                after_return(need_recover);
            }
        }

        void asm_jal(){
            auto ins            = inc::cast<jalx_t>(the.ins);
            auto target         = the.ins.opc == jali ? 
                rim.load(ins.im4_opa, 4/*bit*/).read_with_clear<u64>() :
                regs[ins.im4_opa].ru64;

            // 忽略第 0 位
            auto address        = seg_t(target & seg_t::mask_code_segment);

            // 跨程序段调用
            if (address.segment){
                // TODO====================================================================================
            }

            // 按需保存通用寄存器位组
            // sta.psm 存放着调用栈期望保存的寄存器
            // 每个函数可能有不同的保存期望，sta.psm 每经过一次调用就整合当前期望保存的寄存器
            // 如果设置 ins.ps 位，则指示 jalx 下一条指令是期望保存寄存器的位组
            if (ins.ps){
                auto ps         = sta.ps;
                wrmem(& ps, cs.address, sizeof(ps));
                rdmem(& ps, pc.address + sizeof(ins_t), sizeof(ins_t));
                sta.psm        |= ps;
                cs.address     += sizeof(ps);
            }

            // 按需保存状态寄存器
            if (ins.sf){
                wrmem(& sta.flag, cs.address, sizeof(sta.flag));
                cs.address     += sizeof(sta.flag);
            }

            // 段内跨区域跳转
            // 让 position 排后边，作为第一个读到的 u16，在根据 over_area 位判断是否存在 area
            if (address.area){
                wrmem(& pc.area, cs.address, sizeof(pc.area));
                pc.over_area    = true;
                pc.area         = address.area;
                cs.address     += sizeof(pc.area);
            }

            wrmem(& pc.position, cs.address, sizeof(pc.position));
            pc.position         = address.position;
            cs.address         += sizeof(pc.position);

            // 当执行 jalx 指令后，此时 pc 会指向子函数第一条指令
            // 接着在 the.run() 函数中完成一轮循环，需要将 pc += sizeof(ins_t)
            // 这里让 pc -= sizeof(ins_t); 保证下一条指令指向子函数第一条指令
            // 在 jalx() 中不直接执行子函数第一条指令，避免恶意的递归调用（第一条指令是 jalx 指令）导致爆栈
            pc.address         -= sizeof(ins_t);

            // 复位 sta.predetermined 避免不必要的副作用
            sta.predetermined   = sta.reset_predetermined;

            // 进入函数调用时需要清空立即数寄存器
            rim.clear();
        }

        void asm_bdc(){
            auto i      = inc::cast<bdc_t>(ins);
            auto assign = [&](res_t mode, auto value){
                for(uxx mask = i.bmp, idx = 0; mask != 0; mask >>= 1, idx++){
                    if (0 == (mask & 1)){
                        continue;
                    }

                    auto j = i.bank << 2 | idx;
                    this->before_modify_register(j);
                    this->mode[j] = mode;
                    this->regs[j] = value;

                    if (mode != res_t::is_f32){
                        continue;
                    }

                    this->regs[j].rh32 = 0;
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

        void asm_mov(){
            #define a   regs[ins.opa]
            #define b   regs[ins.opb]
            #define t   regs[ins.opt]

            switch(before_modify_register(ins.opa); cmd_t(ins.opc)){
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

        // TODO:rduxr
        // TODO:rdpri
        // TODO:wruxr
        // TODO:wrpri

        void asm_ld(){
            auto i                  = inc::cast<ldx_t>(ins);
            auto has_sign           = false;
            auto is_float           = ins.opc >= cmd_t::lds;
            auto bytes              = is_float ? 
                4 << i.scale/*mf32/mf64*/ :
                1 << i.scale/*m08/m16/m32/m64*/;
            auto address            = i.with_rt ? 
                regs[ins.opb].ru64 + regs[ins.opt].ru64 : 
                regs[ins.opb].ru64;

            before_modify_register(ins.opa);

            // 先设置类型
            // 浮点类型
            if (is_float){
                mode[ins.opa]       = ins.opc & res_t::is_f64 ? res_t::is_f64 : res_t::is_f32;
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

        void asm_ldk(){
            auto ins           = inc::cast<rwss_t>(the.ins);
            auto imm           = rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>();
            auto addr          = ss.address + imm * sizeof(reg_t);

            // 先设置寄存器类型
            before_modify_register(ins.opa);
            mode[ins.opa]      = res_t(ins.type);
            rdmem(& regs[ins.opa], addr, sizeof(reg_t));

            if (ins.side_effect){
                ss.address     = addr;
            }
        }

        void asm_st(){
            auto i                  = inc::cast<stxx_t>(ins);
            auto bytes              = 1 << i.scale;
            auto address            = i.with_rt ? 
                regs[ins.opb].ru64 + regs[ins.opt].ru64 : 
                regs[ins.opb].ru64;
            wrmem(& regs[ins.opa], address, bytes);
        }

        void asm_stk(){
            auto ins           = inc::cast<rwss_t>(the.ins);
            auto imm           = rim.load(ins.im4, 4/*bits*/).read_with_clear<u64>();
            auto addr          = ss.address - imm * sizeof(reg_t);
            wrmem(& regs[ins.opa], addr, sizeof(reg_t));

            if (ins.side_effect){
                ss.address     = addr;
            }
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

        void asm_add(){
            f8([&](auto & a, auto b, auto c){
                if constexpr (inc::is_float<decltype(a)>){
                    a = b + c;
                }
                else{
                    add(a, b, c);
                }
            });
        }

        void asm_sub(){
            f8([&](auto & a, auto b, auto c){
                if constexpr (inc::is_float<decltype(a)>){
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
                        sta.uf  = ~m.high != 0;
                    }
                    else {
                        if (inc::is_signed<u_t> and b < 0){
                            m   = inc::mul(u64(-b), u64(-c));
                        }
                        else{
                            m   = inc::mul(u64(b), u64(c));
                        }

                        sta.cf  = m.high != 0;
                        sta.uf  = 0;
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
                    sta.uf          = b < 0;
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
                    inc::swap(xref(ra), xref(rb));
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

        void asm_imm(){
            auto im = inc::cast<u16>(ins) & 0xfff;
            rim.load(im, 12/*bits*/);
        }
    };

    
}

#endif

xexport_space(mixc::extern_isa_vcpu::origin)
