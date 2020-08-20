#ifndef xpack_extern_isa_cpu
#define xpack_extern_isa_cpu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_isa_cpu::origin
#include"define/base_type.hpp"
#include"macro/xdefer.hpp"
#include"memop/cast.hpp"
#include"memop/swap.hpp"
#include"meta/is_float.hpp"
#include"meta/is_signed.hpp"
#include"meta/is_integer.hpp"
#include"instruction/add.hpp"
#include"instruction/index_of_last_set.hpp"
#include"instruction/mul.hpp"
#include"instruction/shift_left.hpp"
#include"instruction/shift_right.hpp"
#include"instruction/sub.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_isa_cpu::origin{
    enum cmd_t : u08{
        // 立即数加载
        imm             = 0,

        // 赋值指令
        // f32 <- u08/i08/u16/i16/u32/i32/u64/i64
        movsb           = 16,
        movsbx          ,
        movsw           ,
        movswx          ,
        movsd           ,
        movsdx          ,
        movsq           ,
        movsqx          ,

        // 赋值指令
        // f64 <- u08/i08/u16/i16/u32/i32/u64/i64
        movfb           ,
        movfbx          ,
        movfw           ,
        movfwx          ,
        movfd           ,
        movfdx          ,
        movfq           ,
        movfqx          ,

        // 赋值指令
        // u64 <- u08/u16/u32/u64
        // i64 <- i08/i16/i32/i64
        movqb           ,
        movqbx          ,
        movqw           ,
        movqwx          ,
        movqd           ,
        movqdx          ,
        movqq           ,
        movqqx          ,

        // 赋值指令
        // f32 <- f32/f64
        // f64 <- f32/f64
        // u64 <- f32/f64
        // i64 <- f32/f64
        movss           ,
        movsf           ,
        movfs           ,
        movff           ,
        movqs           ,
        movqf           ,
        movqsx          ,
        movqfx          ,

        // 广播式赋值
        bdcss           ,
        bdcff           = bdcss + 4,
        bdcqq           = bdcff + 4,
        bdcqqx          = bdcqq + 4,

        // 转移指令
        ifge            = bdcqqx + 4,
        ifgt            ,
        ifle            ,
        iflt            ,
        ifeq            ,
        ifne            ,
        ifz             ,
        ifnz            ,
        ifcf            ,
        ifnc            ,
        ifof            ,
        ifno            ,
        jmp             ,
        ret             ,
        jali            ,
        jalr            ,

        // 读取内存
        // u64 <- mu08/mu16/mu32/mu64
        // i64 <- mi08/mi16/mi32/mi64
        // f32 <- mf32
        // f64 <- mf64
        ldb             ,
        ldbx            ,
        ldbt            ,
        ldbtx           ,
        ldw             ,
        ldwx            ,
        ldwt            ,
        ldwtx           ,
        ldd             ,
        lddx            ,
        lddt            ,
        lddtx           ,
        ldq             ,
        ldqx            ,
        ldqt            ,
        ldqtx           ,
        lds             ,
        ldf             ,
        ldst            ,
        ldft            ,
        // idx_rsv0,
        // idx_rsv1,
        pop             = ldft + 3,
        pops            ,

        stb             ,
        stbt            ,
        stw             ,
        stwt            ,
        std             ,
        stdt            ,
        stq             ,
        stqt            ,
        // stx_rsv0,
        // stx_rsv1,
        // stx_rsv2,
        // stx_rsv3,
        // stx_rsv4,
        // stx_rsv5,
        push            = stq + 6,
        pushs           ,

        // 算数、逻辑运算、比较
        add             ,
        sub             = add   + 8,
        mul             = sub   + 8,
        div             = mul   + 8,
        sft             = div   + 8,
        cmp             = sft   + 8,
        band            = cmp   + 4,
        bor             = band  + 4,
        bxor            = bor   + 4,
        bnand           = bxor  + 4,

        // 读取用户扩展寄存器
        rduxr           = bnand + 4,

        // 写入用户扩展寄存器
        wruxr           ,
    };

    enum rduxr_t{
        st              , // 读取临时 f32 结果寄存器
        ft              , // 读取临时 f64 结果寄存器
        qt              , // 读取临时 i64/u64 结果寄存器
        sta             , // 读取状态寄存器
        mod             , // 预定除法余数
        proh            , // 预定乘法高位
        sfto            , // 预定移位溢出位组
    };

    enum f4_t{
        f4aab,
        f4tab,
        f4aat,
        f4tai,
    };

    enum c4_t{
        c4ab,
        c4ai,
        c4ia,
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

    struct cpu{
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
                    if (inc::index_of_last_set(imm) == total_bits - 1){
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
        };

        typedef enum register_state_t{
            is_f32,
            is_f64,
            is_u64,
            is_i64,
        } res_t;

        struct mov_t{
            u08  with_sign_extern   : 1;
            u08  scale              : 2;
            u08  target_type        : 2;
        };

        struct mov2_t{
            u08 is_f32              : 1;
            u08 is_target           : 2;
        };

        struct bor_t{
            u64 shift               : 6;
            u64 mask                : 6;
        };

        struct sta_t{
            u64 gt                  : 1;
            u64 eq                  : 1;
            u64 zf                  : 1;

            // 上溢
            u64 cf                  : 1;

            // 下溢
            u64 of                  : 1;

            // 预设(predetermined)
            u64 pmod                : 5;
            u64 pmulh               : 5;
            u64 psfto               : 5;
        };

        // 段偏式
        struct seg_t{
            u64 offset              : 32;
            u64 segment             : 32;
        };

        enum{
            general_purpose_register_count  = 0x10,
            no_predetermined                = 0x10,
        };

        imm_t   rim;    // 立即数寄存器
        ins_t   ins;    // 指令寄存器
        reg_t   regs[general_purpose_register_count];
        reg_t   rs;     // 临时 f32 寄存器
        reg_t   rf;     // 临时 f64 寄存器
        reg_t   rq;     // 临时 r64 寄存器
        sta_t   sta;    // 状态寄存器
        seg_t   pc;     // 程序计数器
        res_t   mode[general_purpose_register_count];

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

        void mov(){
            auto & ra   = regs[ins.opa];
            auto & rb   = regs[ins.opb];
            auto   i    = ins.opc - movsb/*begin*/;

            if (ins.opc >= movss){
                auto   info = inc::cast<mov_t>(i);
                auto   m    = res_t(info.target_type != is_u64 ? 
                    info.target_type/*f32/f64*/ : 
                    info.target_type/*uint*/ + info.with_sign_extern/*is int if this value eq 1*/
                );

                switch(mode[ins.opa] = m){
                case is_f32:
                    ra.rf32 = info.with_sign_extern ? 
                        sign_extern(rb, info.scale) :
                        zero_extern(rb, info.scale);
                    break;
                case is_f64:
                    ra.rf64 = info.with_sign_extern ? 
                        sign_extern(rb, info.scale) :
                        zero_extern(rb, info.scale);
                    break;
                default:
                    ra.ri64 = info.with_sign_extern ? 
                        sign_extern(rb, info.scale) :
                        zero_extern(rb, info.scale);
                    break;
                }
            }
            else{
                auto info = inc::cast<mov2_t>(i);

                switch(mode[ins.opa] = res_t(info.is_target)){
                case is_f32: ra.rf32 = info.is_f32 ? f32(rb.rf32) : f32(rb.rf64); ra.rh32 = 0; return;
                case is_f64: ra.rf64 = info.is_f32 ? f64(rb.rf32) : f64(rb.rf64); return;
                case is_u64: ra.ru64 = info.is_f32 ? f64(rb.rf32) : f64(rb.rf64); return;
                default:     ra.ri64 = info.is_f32 ? f64(rb.rf32) : f64(rb.rf64); return;
                }
            }
        }

        void ifx(bool match){
            auto offset = rim.load(ins.im, 8/*bits*/).read_with_clear<u64>();

            if (not match){
                pc.offset += offset * sizeof(ins_t);
            }
        }

        #define xgen(name,...)  void name(){ ifx(__VA_ARGS__); }

        xgen(ifge, sta.gt or sta.eq)
        xgen(ifgt, sta.gt)
        xgen(ifle, not (sta.gt))
        xgen(iflt, not (sta.gt or sta.eq))
        xgen(ifeq, sta.eq)
        xgen(ifne, not sta.eq)
        xgen(ifz , sta.zf)
        xgen(ifnz, not sta.zf)
        xgen(ifcf, sta.cf)
        xgen(ifnc, not sta.cf)
        xgen(ifof, sta.of)
        xgen(ifno, not sta.of)
        xgen(jmp , false/*force*/)

        void jal(){
            // 
        }

        static constexpr bool with_hidden_imm = true;

        #define xgen(rt,type)                                                                                                                   \
        switch(m){                                                                                                                              \
        case f8abti: invoke(ra.r ## type , rb.r ## type , rt           , with_hidden_imm ? rim.read_with_clear<type>() : type(0)); return;      \
        case f8atbi: invoke(ra.r ## type , rt           , rb.r ## type , with_hidden_imm ? rim.read_with_clear<type>() : type(0)); return;      \
        case f8aabi: invoke(ra.r ## type , ra.r ## type , rb.r ## type , with_hidden_imm ? rim.read_with_clear<type>() : type(0)); return;      \
        case f8tabi: invoke(rt           , ra.r ## type , rb.r ## type , with_hidden_imm ? rim.read_with_clear<type>() : type(0)); return;      \
        }                                                                                                                                       \
                                                                                                                                                \
        rim.load(ins.opb, 4/*bits*/);                                                                                                           \
                                                                                                                                                \
        switch(m){                                                                                                                              \
        case f8aai : invoke(ra.r ## type , ra.r ## type, rim.read_with_clear<type>(), 0); return;                                               \
        case f8aia : invoke(ra.r ## type , rim.read_with_clear<type>(), ra.r ## type, 0); return;                                               \
        case f8tai : invoke(rt           , ra.r ## type, rim.read_with_clear<type>(), 0); return;                                               \
        case f8tia : invoke(rt           , rim.read_with_clear<type>(), ra.r ## type, 0); return;                                               \
        }

        template<class opr>
        void f8(bool with_hidden_imm, opr const & invoke){
            auto & ra = regs[ins.opa];
            auto & rb = regs[ins.opb];
            auto   m  = f8_t(ins.opc & 0x7);

            switch(res_t(mode[ins.opa])){
            case is_f32: xgen(rs.f32, f32) break;
            case is_f64: xgen(rf.f64, f64) break;
            case is_u64: xgen(rq.u64, u64) break;
            case is_i64: xgen(rq.i64, i64) break;
            }
        }

        template<class opr>
        void f8qx(bool with_hidden_imm, opr const & invoke){
            auto & ra = regs[ins.opa];
            auto & rb = regs[ins.opb];
            auto   m  = f8_t(ins.opc & 0x7);

            if (mode[ins.opa] == is_i64){
                xgen(rq.i64, i64)
            }
            else{
                xgen(rq.u64, u64)
            }
            #undef xgen
        }

        template<class type>
        void add(type & a, type b, type c){
            u128 m      = inc::add(b, c);
            a           = type(m.low);

            if constexpr (inc::is_signed<type>){
                auto x0 = u64(b & c) >> 63;
                auto x1 = u64(b | c) >> 63;

                // 结果值的符号位
                auto x2 = u64(a) >> 63;

                // 下溢：同时为负数时，结果符号位变化
                sta.cf  = (x0 != 0 and x2 == 0);

                // 上溢：同时为正数，结果符号位变化
                sta.of  = (x1 == 0 and x2 == 1);
            }
            else{
                sta.cf  = (m.high);
                sta.of  = (0);
            }
        }

        void add(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                if (i == 0){
                    i = 1;
                }
                if (c *= i; inc::is_float<decltype(a)>){
                    a = b + c;
                }
                else{
                    add(a, b, c);
                }
            });
        }

        void sub(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                if (i == 0){
                    i = 1;
                }
                if (c *= i; inc::is_float<decltype(a)>){
                    a = b - c;
                }
                else{
                    add(a, b, 0 - c);
                }

                sta.gt = b > c;
                sta.eq = b == c;
            });
        }

        void mul(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                using ut = decltype(b);

                if constexpr (inc::is_float<ut>){
                    a = b * c + i;
                }
                else{
                    u128 m; 

                    // 符号位不相等结果为负数，需要符号位扩展
                    if (inc::is_signed<ut> and u64(b ^ c) >> 63){
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

                    if (i == 0){
                        ; // pass
                    }
                    else if (not inc::is_signed<ut> or i > 0){
                        u128 x  = inc::add(m.low, u64(i));
                        m.low   = x.low;
                        m.high  = m.high + x.high;
                    }
                    else{
                        u128 x  = inc::sub(m.low, u64(i));
                        m.low   = x.low;
                        m.high  = m.high - x.high;
                    }

                    if (a = m.low; sta.pmulh != no_predetermined){
                        regs[sta.pmulh] = m.high;
                        sta.pmulh       = no_predetermined;
                    }
                }
            });
        }

        void div(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                using ut = decltype(b);
                a        = b / c + i;

                if constexpr (inc::is_integer<ut>){
                    if (sta.pmod != no_predetermined){
                        regs[sta.pmod] = b % c;
                        sta.pmod = no_predetermined;
                    }
                }
            });
        }

        void sft(){
            f8qx(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                // 默认时保留 a 中的所有位
                if (i == 0){
                    i = decltype(i)(-1);
                }
                else{
                    i = (1ull << i) - 1;
                }

                u64 overflow_part;
                i64 bits = i64(c);

                if (bits >= 0){
                    u128 m          = inc::shift_right(u64(b), u64(bits));
                    overflow_part   = m.low;
                    a               = m.high;
                    sta.cf          = overflow_part >> 63;

                    if constexpr (inc::is_signed<decltype(b)>){
                        if (a != 0){
                            a |= u64(-1) << inc::index_of_last_set(a);
                        }
                    }
                }
                else{
                    u128 m          = inc::shift_left(u64(b), u64(-bits));
                    overflow_part   = m.high;
                    a               = m.low;
                    sta.cf          = overflow_part & 1;
                }

                a                 &= i;
                sta.zf             = a == 0;

                if (sta.psfto != no_predetermined){
                    regs[sta.psfto] = overflow_part;
                    sta.psfto       = no_predetermined;
                }
            });
        }

        void cmp(){
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

        #define xgen(...)                                                  \
        f8qx(with_hidden_imm, [&](auto & a, auto b, auto x, auto i){       \
            auto  m = (inc::cast<bor_t>(i));                               \
            auto  c = (u64(x) >> m.shift) & (u64(-1)) >> m.mask);          \
            a       = __VA_ARGS__;                                         \
            sta.zf  = a == 0;                                              \
        });

        void band(){
            xgen(b & c)
        }

        void bor(){
            xgen(b | c)
        }

        void bnand(){
            xgen(~(b & c))
        }

        void bxor(){
            xgen(b ^ c)
            #undef xgen
        }


    };
}

#endif
