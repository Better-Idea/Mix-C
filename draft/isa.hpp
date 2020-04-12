#ifndef xpack_draft_isa
#define xpack_draft_isa
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::draft_isa
        #include"define/base_type.hpp"
        #include"memop/cast.hpp"
        #include"memop/signature.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::draft_isa{
        enum opviii_t{
            o8_aai,
            o8_aaii,
            o8_aaiii,
            o8_aaiv,
            o8_aab,
            o8_abt,
            o8_atb,
            o8_tab,
        };

        enum opvi_t{
            o6_aai,
            o6_aab,
            o6_abt,
            o6_tab,
            o6_tia = 0,
            o6_atb,
        };

        #define xgen(name,count)  name, name ## _end = name + count - 1

        enum set_t : u08{
            xgen(ldi    , 16),

            xgen(ldxi   , 4),   // ldx.i    ra, [rb + im]
            xgen(ldxit  , 4),   // ldx.i    ra, [rb + im + rt]
            xgen(ldxu   , 4),   // ldx.u    ra, [rb + im]
            xgen(ldxut  , 4),   // ldx.u    ra, [rb + im + rt]

            xgen(stxu   , 4),   // stx.u    ra, [rb + im]
            xgen(stxut  , 4),   // stx.u    ra, [rb + im + rt]

            xgen(psh    , 2),
            xgen(pop    , 2),
            xgen(ldf    , 2),   // ldf      sa/da, [rb + im]
            xgen(ldft   , 2),   // ldf      sa/da, [rb + im + rt]
            xgen(stf    , 2),   // stf      sa/da, [rb + im]
            xgen(stft   , 2),   // stf      sa/da, [rb + im + rt]
            xgen(pshf   , 2),
            xgen(popf   , 2),

            xgen(shl    , 8),
            xgen(shr    , 8),
            xgen(sar    , 8),
            xgen(bts    , 8),

            xgen(movbx  , 1),   // movx ra, rb.b
            xgen(movwx  , 1),   // movx ra, rb.w
            xgen(movdx  , 1),   // movx ra, rb.d
            xgen(movix  , 1),   // movx ra, im
            xgen(movb   , 1),   // mov  ra, rb.b
            xgen(movw   , 1),   // mov  ra, rb.w
            xgen(movd   , 1),   // mov  ra, rb.d
            xgen(movq   , 1),   // mov  ra, rb

            xgen(movfsi , 1),   // mov  sa, im
            xgen(movfs  , 1),   // mov  sa, sb
            xgen(movfdi , 1),   // mov  da, im
            xgen(movfd  , 1),   // mov  da, db
            xgen(uq2fs  , 1),   // cast sa, rb
            xgen(uq2fd  , 1),   // cast da, rb
            xgen(iq2fs  , 1),   // cast sa, rb.i
            xgen(iq2fd  , 1),   // cast da, rb.i

            xgen(add    , 4),
            xgen(adc    , 4),
            xgen(sub    , 4),
            xgen(sbb    , 4),
            xgen(mul    , 4),
            xgen(mulu   , 4),
            xgen(mulx   , 4),
            xgen(mulxu  , 4),
            xgen(div    , 4),
            xgen(divu   , 4),
            xgen(subp   , 2),
            xgen(sbbp   , 2),
            xgen(divp   , 2),
            xgen(divup  , 2),

            xgen(andi   , 4),
            xgen(ori    , 4),
            xgen(nandi  , 4),
            xgen(xori   , 4),

            xgen(cmp    , 2),
            xgen(fcmp   , 2),
            xgen(mask   , 2),  // mask bits
            xgen(ldf    , 1),  // load bits range
            xgen(stf    , 1),  // store bits range
            xgen(sbc    , 1),  // set bit count
            xgen(ios    , 1),  // index of set
            xgen(lis    , 1),  // last index of set
            xgen(___    , 1),
            xgen(swap   , 1),
            xgen(lock   , 1),

            xgen(ifeq   , 1),
            xgen(ifne   , 1),
            xgen(ifcf   , 1),
            xgen(ifnc   , 1),
            xgen(ifab   , 1),
            xgen(ifae   , 1),
            xgen(ifbt   , 1),
            xgen(ifbe   , 1),
            xgen(ifgt   , 1),
            xgen(ifge   , 1),
            xgen(iflt   , 1),
            xgen(ifle   , 1),
            xgen(ifov   , 1),
            xgen(ifno   , 1),
            xgen(jmp    , 2),
            xgen(call   , 2),

            xgen(fadd   , 4),
            xgen(fsub   , 4),
            xgen(fmul   , 4),
            xgen(fdiv   , 4),
            xgen(fsubp  , 2),
            xgen(fdivp  , 2),
        };

        enum subset_iv_t{
            xgen(ret    , 1),
            xgen(rgn    , 1), // register windoes region
        };

        #undef  xgen

        using u128 = __uint128_t;

        struct cpu_t{
            using the_t = cpu_t;

            enum{
                integer_register_count = 16,
            };

            struct opc_t{
                union{
                    struct{
                        u08 a   : 4;
                        u08 b   : 4;
                    };
                    u08 ab;
                    u08 im4 : 4;
                };
                u08 op;

                operator u16 (){
                    return u16p(this)[0];
                }
            } ins;

            union state_t{
                u64     zf  : 1;
                struct{
                    u64 eq  : 1;
                    u64 cf  : 1;
                    u64 ab  : 1;
                    u64 gt  : 1;
                };
            } sta;

            struct imm_t{
                using the_t = imm_t;

                imm_t & load(u64 value, uxx width){
                    v    |= value << bits;
                    bits += width;

                    if (bits > 64){
                        bits = 64;
                    }
                    return the;
                }

                operator u64(){
                    auto t = v;
                    bits   = 0;
                    v      = 0;
                    return t;
                }

                operator i64(){
                    if (u64 mask = u64(-1) << bits; v & mask){
                        v |= mask;
                    }
                    return i64(u64(the));
                }

                operator f32(){
                    auto b = bits;
                    auto u = u32(the.operator u64());

                    if (b < 32){
                        u <<= 32 - b;
                    }
                    return inc::cast<f32>(u);
                }
                
                operator f32(){
                    auto b = bits;
                    auto u = the.operator u64();

                    if (b < 64){
                        u <<= 64 - b;
                    }
                    return inc::cast<f64>(u);
                }
            private:
                u64 v       = 0;
                uxx bits    = 0;
            } imm;

            union reg_t {
                u64 u;
                i64 i;
            }     r[integer_register_count], 
                & sp = r[0], // stack point
                & t  = r[1], // temp
                & x  = r[2]; // extern

            struct cmd_t{
                void operator()(void * cpu){
                    inc::signature<void>::call(& cpu, cmd);
                }

                template<class lambda>
                void operator= (lambda const & func){
                    cmd = inc::signature<void>::check(& lambda::operator());
                }
            private:
                voidp cmd;
            } sets[256];

            template<class type, class lambda>
            void opviii(type * r, lambda && op){
                switch(opviii_t(ins.op & 0x7)){
                case o8_aai:    op(r[ins.a], r[ins.a], imm.load(ins.im4 + 0x00, 4)); break;
                case o8_aaii:   op(r[ins.a], r[ins.a], imm.load(ins.im4 + 0x10, 5)); break;
                case o8_aaiii:  op(r[ins.a], r[ins.a], imm.load(ins.im4 + 0x20, 6)); break;
                case o8_aaiv:   op(r[ins.a], r[ins.a], imm.load(ins.im4 + 0x30, 6)); break;
                case o8_aab:    op(r[ins.a], r[ins.a], r[ins.b]);                    break;
                case o8_abt:    op(r[ins.a], r[ins.b], r[ins.t]);                    break;
                case o8_atb:    op(r[ins.a], r[ins.t], r[ins.b]);                    break;
                case o8_tab:    op(r[ins.t], r[ins.a], r[ins.b]);                    break;
                }
            }

            template<class type, class lambda>
            void opiv(type * r, lambda && op){
                switch(opvi_t(ins.op & 0x3)){
                case o6_aai:    op(r[ins.a], r[ins.a], imm.load(ins.im4, 4));        break;
                case o6_aab:    op(r[ins.a], r[ins.a], r[ins.b]);                    break;
                case o6_abt:    op(r[ins.a], r[ins.b], r[ins.t]);                    break;
                case o6_tab:    op(r[ins.t], r[ins.a], r[ins.b]);                    break;
                }
            }

            template<class type, class lambda>
            void opivp(type * r, lambda && op){
                switch(opvi_t(ins.op & 0x1)){
                case o6_tia:    op(r[ins.t], r[ins.i], r[ins.a]);                    break;
                case o6_atb:    op(r[ins.a], r[ins.t], r[ins.b]);                    break;
                }
            }

            void builtin_sub(reg_t & a, reg_t b, reg_t c, uxx mask){
                u128 u = u128(b.u) - c.u - (sta.cf & mask);
                a.u    = u64(u);
                sta.cf = u64(u >> 64) & 1;
                sta.zf = a.u == 0;
                sta.ab = b.u > c.u;
                sta.gt = b.i > c.i;
            }

            void builtn_add(reg_t & a, reg_t b, reg_t c, uxx mask){
                u128 u = u128(b.u) + c.u + (sta.cf & mask);
                a.u    = u64(u);
                sta.cf = u64(u >> 64) & 1;
                sta.zf = a.u == 0;
            }

            cpu_t(){
                #define xgen(cmd)   for(uxx __i = cmd; __i <= cmd ## _end; __i++) sets[__i] = [this]()
                
                xgen(ldi){
                    imm.load(ins & 0xfff, 12);
                };

                xgen(ldxi){
                    
                };

                xgen(ldxit){
                    
                };

                xgen(ldxu){
                    
                };

                xgen(ldxut){
                    
                };

                xgen(stxu){
                    
                };

                xgen(stxut){
                    
                };

                xgen(psh){
                    
                };

                xgen(pop){
                    
                };

                xgen(ldf){
                    
                };

                xgen(ldft){
                    
                };

                xgen(stf){
                    
                };

                xgen(stft){
                    
                };

                xgen(pshf){
                    
                };

                xgen(popf){
                    
                };

                #define xgen2(bits)     \
                    if (c.u == 0){      \
                        return;         \
                    }                   \
                    if (c.u > bits){    \
                        x.u     = 0;    \
                        a.u     = 0;    \
                        sta.cf  = 0;    \
                        sta.zf  = 1;    \
                    } else

                xgen(shl){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        xgen2(64) {
                            x.u    = b.u >> (64 - c.u);
                            a.u    = b.u << c.u;
                            sta.zf = a.u == 0;
                            sta.cf = x.u & 1;
                        }
                    });
                };

                xgen(shr){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        xgen2(64) {
                            x.u    = b.u >> (c.u - 1);
                            a.u    = b.u << c.u;
                            sta.zf = a.u == 0;
                            sta.cf = x.u & 1;
                        }
                    });
                };

                xgen(sar){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        xgen2(63){
                            x.u    = b.i >> (c.u - 1);
                            a.u    = b.i << c.u;
                            sta.zf = a.u == 0;
                            sta.cf = x.u & 1;
                        }
                    });
                };

                xgen(bts){

                };

                xgen(movbx){

                };

                xgen(movwx){

                };

                xgen(movdx){

                };

                xgen(movix){

                };

                xgen(movb){

                };

                xgen(movw){

                };

                xgen(movd){

                };

                xgen(movq){

                };

                xgen(movfsi){

                };

                xgen(movfs){

                };

                xgen(movfdi){

                };

                xgen(movfd){

                };

                xgen(uq2fs){

                };

                xgen(uq2fd){

                };

                xgen(iq2fs){

                };

                xgen(iq2fd){

                };

                xgen(add){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtn_add(a, b, c, false);
                    });
                };

                xgen(adc){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtn_add(a, b, c, true);
                    });
                };

                xgen(sub){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_sub(a, b, c, false);
                    });
                };

                xgen(sbb){
                    opviii(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_sub(a, b, c, true);
                    });
                };

                xgen(mul){

                };

                xgen(mulu){

                };

                xgen(mulx){

                };

                xgen(mulxu){

                };

                xgen(div){

                };

                xgen(divu){

                };

                xgen(subp){

                };

                xgen(sbbp){

                };

                xgen(divp){

                };

                xgen(divup){

                };

                xgen(andi){

                };

                xgen(ori){

                };

                xgen(nandi){

                };

                xgen(xori){

                };

                xgen(cmp){

                };

                xgen(fcmp){

                };

                xgen(mask){

                };

                xgen(ldf){

                };

                xgen(stf){

                };

                xgen(sbc){

                };

                xgen(ios){

                };

                xgen(lis){

                };

                xgen(___){

                };

                xgen(swap){

                };

                xgen(lock){

                };

                xgen(ifeq){

                };

                xgen(ifne){

                };

                xgen(ifcf){

                };

                xgen(ifnc){

                };

                xgen(ifab){

                };

                xgen(ifae){

                };

                xgen(ifbt){

                };

                xgen(ifbe){

                };

                xgen(ifgt){

                };

                xgen(ifge){

                };

                xgen(iflt){

                };

                xgen(ifle){

                };

                xgen(ifov){

                };

                xgen(ifno){

                };

                xgen(jmp){

                };

                xgen(call){

                };

                xgen(fadd){

                };

                xgen(fsub){

                };

                xgen(fmul){

                };

                xgen(fdiv){

                };

                xgen(fsubp){

                };

                xgen(fdivp){

                };

                #undef  xgen
            }
        };
        
    }

#endif

