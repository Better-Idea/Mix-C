#ifndef xpack_extern_isa_cpu
#define xpack_extern_isa_cpu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_isa_cpu::origin
#include"define/base_type.hpp"
#include"macro/xdefer.hpp"
#include"memop/cast.hpp"
#include"memop/swap.hpp"
#include"meta/is_same.hpp"
#include"instruction/index_of_last_set.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_isa_cpu::origin{
    enum cmd_t{
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

        // 广播赋值
        bdcss           ,
        bdcff           = bdcss + 4,
        bdcqq           = bdcff + 4,
        bdcqqx          = bdcqq + 4,

        // 转移指令
        ifae            = bdcqqx + 4,
        ifat,
        ifbe,
        ifbt,
        ifge,
        ifgt,
        ifle,
        iflt,
        ifeq,
        ifz             = ifeq,
        ifne,
        ifnz            = ifne,
        ifcf,
        ifnc,
        ifof,
        jmp,
        jal,
        ret,

        // 读取内存
        // u64 <- mu08/mu16/mu32/mu64
        // i64 <- mi08/mi16/mi32/mi64
        // f32 <- mf32
        // f64 <- mf64
        ldb,
        ldbx,
        ldw,
        ldwx,
        ldd,
        lddx,
        ldq,
        ldqx,
        lds,
        ldf,
        // idx_rsv0,
        // idx_rsv1,
        // idx_rsv2,
        // idx_rsv3,
        pop             = ldf + 5,
        pops,

        stb,
        stw,
        std,
        stq,
        // stx_rsv0,
        // stx_rsv1,
        push            = stq + 3,
        pushs,

        // 算数、逻辑运算、比较
        add,
        sub             = add   + 8,
        mul             = sub   + 8,
        div             = mul   + 8,
        sft             = div   + 8,
        cmp             = sft   + 8,
        band            = cmp   + 4,
        bor             = band  + 4,
        bxor            = bor   + 4,
        bnand           = bxor  + 4,
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
                xdefer{
                    imm         = 0;
                    total_bits  = 0;
                };

                if constexpr (inc::is_same<type, f32> or inc::is_same<type, f64>){
                    auto bits = sizeof(type) * 8;
                    if (total_bits < bits){
                        imm <<= bits - total_bits;
                    }
                    return inc::cast<type>(imm);
                }
                else if constexpr (inc::is_same<type, u64> or inc::is_same<type, i64>){
                    if (total_bits == 0){
                        return 0;
                    }
                    if (inc::index_of_last_set(imm) == total_bits){
                        imm |= (u64(1) << total_bits) - 1;
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
            u32     ru32;
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

            enum{ sp = 0, };
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

        struct sta_t{
            u08 cf                  : 1;
            u08 zf                  : 1;
            u08 gt                  : 1;
        };

        enum{
            general_purpose_register_count  = 16,
        };

        imm_t   rim;
        ins_t   ins;
        reg_t   regs[general_purpose_register_count];
        reg_t   rtf32; // 临时 f32 寄存器
        reg_t   rtf64; // 临时 f64 寄存器
        reg_t   rti64; // 临时 i64 寄存器
        sta_t   sta;
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

            // zero exter
            val.ri64 = val.ri64 & mask;
            return val.ri64;
        }

        void mov(){
            auto & ra           = regs[ins.opa];
            auto & rb           = regs[ins.opb];

            if (ins.opc >= movss){
                mode[ins.opa]   = res_t(ins.opc & 0x3);

                if (mode[ins.opa] == is_f32){
                    ra.ru64     = 0;
                    ra.rf32     = rb.rf32;
                }
                else{
                    ra          = rb;
                }
                return;
            }

            auto i          = ins.opc - movsb/*begin*/;
            auto info       = inc::cast<mov_t>(i);
            auto m          = res_t(info.target_type != is_u64 ? 
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
        case f8aai : invoke(ra.r ## type , ra.r ## type, rim.read_with_clear<type>(), 0);              return;                                  \
        case f8aia : invoke(ra.r ## type , rim.read_with_clear<type>(), ra.r ## type, 0);              return;                                  \
        case f8tai : invoke(rt           , ra.r ## type, rim.read_with_clear<type>(), 0);              return;                                  \
        case f8tia : invoke(rt           , rim.read_with_clear<type>(), ra.r ## type, 0);              return;                                  \
        }

        template<class opr>
        void f8(bool with_hidden_imm, opr const & invoke){
            auto & ra = regs[ins.opa];
            auto & rb = regs[ins.opb];
            auto   m  = f8_t(ins.opc & 0x7);

            switch(res_t(mode[ins.opa])){
            case is_f32: xgen(rtf32.f32, f32) break;
            case is_f64: xgen(rtf64.f64, f64) break;
            case is_u64: xgen(rti64.u64, u64) break;
            case is_i64: xgen(rti64.i64, i64) break;
            }
        }

        template<class opr>
        void f8x(bool with_hidden_imm, opr const & invoke){
            auto & ra = regs[ins.opa];
            auto & rb = regs[ins.opb];
            auto   m  = f8_t(ins.opc & 0x7);

            if (mode[ins.opa] == is_i64){
                xgen(rti64.i64, i64)
            }
            else{
                xgen(rti64.u64, u64)
            }
        }
        #undef  xgen

        void add(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                if (i == 0){
                    i = 1;
                }
                a = b + c * i;
            });
        }

        void sub(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                if (i == 0){
                    i = 1;
                }
                c     *= i;
                a      = b - c;
                sta.gt = b > c;
                sta.zf = b == c;
            });
        }

        void mul(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                a = b * c + i;
            });
        }

        void div(){
            f8(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                a = b / c + i;
            });
        }

        void sft(){
            f8x(with_hidden_imm, [&](auto & a, auto b, auto c, auto i){
                // i as mask
                // c 为正数表示右移，为负数表示左移
                if (i == 0){
                    i = decltype(i)(-1);
                }
                else{
                    i = (1ull << i) - 1;
                }

                enum{ bits = 8 * sizeof(b) };

                if (bits < c or c < -bits){
                    a       = 0;
                    sta.cf  = 0;
                }
                else if (c == bits){
                    a       = 0;
                    sta.cf  = (b >> (bits - 1)) & 1;
                }
                else if (c == -bits){
                    a       = 0;
                    sta.cf  = b & 1;
                }
                else if (c >= 0){
                    a       = (b << c) & i;
                    sta.cf  = (b >> (bits - c) & 1;
                }
                else{
                    a       = (b >> ( 0 - c)) & i;
                    sta.cf  = (b >> (-1 - c)) & 1;
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
            case is_f32: sta.zf = f80(type) == f80(rb.rf32); sta.gt = f80(type) > f80(rb.rf32); break;      \
            case is_f64: sta.zf = f80(type) == f80(rb.rf64); sta.gt = f80(type) > f80(rb.rf64); break;      \
            case is_u64: sta.zf = f80(type) == f80(rb.ru64); sta.gt = f80(type) > f80(rb.ru64); break;      \
            case is_i64: sta.zf = f80(type) == f80(rb.ri64); sta.gt = f80(type) > f80(rb.ri64); break;      \
            }

            switch(sa){
            case is_f32: xgen(ra.rf32) break;
            case is_f64: xgen(ra.rf64) break;
            case is_u64: xgen(ra.ru64) break;
            case is_i64: xgen(ra.ri64) break;
            }

            #undef  xgen
        }

        void band(){
            f8x(not with_hidden_imm, [&](auto & a, auto b, auto c, auto){
                a = b & c;
                sta.cf = a != 0;
            });
        }

        void bor(){
            f8x(not with_hidden_imm, [&](auto & a, auto b, auto c, auto){
                a = b | c;
                sta.cf = a != 0;
            });
        }

        void bnand(){
            f8x(not with_hidden_imm, [&](auto & a, auto b, auto c, auto){
                a = ~(b & c);
                sta.cf = a != 0;
            });
        }

        void bxor(){
            f8x(not with_hidden_imm, [&](auto & a, auto b, auto c, auto){
                a = b ^ c;
                sta.cf = a != 0;
            });
        }
        

    };
}

#endif
