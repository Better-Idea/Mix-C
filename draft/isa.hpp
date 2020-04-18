#ifndef xpack_draft_isa
#define xpack_draft_isa
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::draft_isa
        #include"define/base_type.hpp"
        #include"define/bitbind.hpp"
        #include"instruction/index_of_first_set.hpp"
        #include"instruction/index_of_last_set.hpp"
        #include"macro/xprop.hpp"
        #include"memop/cast.hpp"
        #include"memop/signature.hpp"
        #include"memop/swap.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::draft_isa{
        union bit_range_t{
            u64     imm;
            struct{
                u64 first_index   : 6;
                u64 last_index    : 6;
            };

            bit_range_t(u64 v) : 
                imm(v){
            }
        };

        enum opcx_t{
            aai,
            aab,
            abt,
            tab,
            tai,
            abi,
            aib,
            atb,
        };

        struct area_t {
            using the_t = area_t;
            using final = area_t;

            xpubget_pubset(code, uxx){
                xr { return the.code_size; }
                xw { the.code_size = value; }
            };

            xpubget_pubset(stack, uxx){
                xr { return the.stack_size; }
                xw { the.stack_size = value; }
            };

            xpubget_pubset(call_stack, uxx){
                xr { return the.call_stack_size; }
                xw { the.call_stack_size = value; }
            };
        private:
            uxx code_size           = 0;
            uxx stack_size          = 0;
            uxx call_stack_size     = 0;
        };

        struct opc_t{
            union{
                struct{
                    u08 b   : 4;
                    u08 a   : 4;
                };
                u08     ab;
                u08     im4 : 4;
            };
            u08         op;

            static constexpr uxx sp     = 0;
            static constexpr uxx bp     = 1;
            static constexpr uxx t      = 2;
            static constexpr uxx x      = 3;

            opc_t(){}
            opc_t(u08 op) : op(op) {}
            opc_t(u08 op, u08 ab) : op(op), ab(ab) {}
            opc_t(u08 op, u08 a, u08 b) : op(op), a(a), b(b){}

            operator u16 (){
                return u16p(this)[0];
            }
        };

        #define xgen(name,count)  name, name ## _end = name + count - 1

        enum set_t : u08{
            xgen(ldi    , 16),

            xgen(shl    , 8),
            xgen(shr    , 8),
            xgen(sar    , 8),
            xgen(bts    , 8),

            xgen(ldib   , 1),
            xgen(ldiw   , 1),
            xgen(ldid   , 1),
            xgen(ldiq   , 1),
            xgen(ldub   , 1),
            xgen(lduw   , 1),
            xgen(ldud   , 1),
            xgen(lduq   , 1),

            xgen(ldtib  , 1),
            xgen(ldtiw  , 1),
            xgen(ldtid  , 1),
            xgen(ldtiq  , 1),
            xgen(ldtub  , 1),
            xgen(ldtuw  , 1),
            xgen(ldtud  , 1),
            xgen(ldtuq  , 1),

            xgen(stb    , 1),
            xgen(stw    , 1),
            xgen(std    , 1),
            xgen(stq    , 1),
            xgen(sttb   , 1),
            xgen(sttw   , 1),
            xgen(sttd   , 1),
            xgen(sttq   , 1),

            xgen(ldfs   , 1),
            xgen(ldfd   , 1),
            xgen(ldtfs  , 1),
            xgen(ldtfd  , 1),
            xgen(stfs   , 1),
            xgen(stfd   , 1),
            xgen(sttfs  , 1),
            xgen(sttfd  , 1),

            xgen(pop    , 2),
            xgen(popf   , 2),
            xgen(psh    , 2),
            xgen(pshs   , 2),

            xgen(pshf   , 2),
            xgen(pshfs  , 2),
            xgen(bdc    , 4),   // broadcast move

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
            xgen(sbb    , 8),
            xgen(sub    , 8),
            xgen(mul    , 4),
            xgen(mulu   , 4),
            xgen(mulx   , 4),
            xgen(mulxu  , 4),
            xgen(div    , 8),
            xgen(divu   , 8),

            xgen(andi   , 4),
            xgen(ori    , 4),
            xgen(nandi  , 4),
            xgen(xori   , 4),

            xgen(mask   , 1),   // mask bits
            xgen(ldbs   , 1),   // load bits range
            xgen(stbs   , 1),   // store bits range
            xgen(sbc    , 1),   // set bit count
            xgen(ios    , 1),   // index of set
            xgen(lis    , 1),   // last index of set
            xgen(swp    , 1),
            xgen(swpf   , 1),

            xgen(subset , 1),
            xgen(rds    , 1),   // read special register
            xgen(___    , 5),
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
            xgen(jmp    , 1),
            xgen(ret    , 1),
            xgen(call   , 1),
            xgen(calli  , 1),

            xgen(addf   , 4),
            xgen(mulf   , 4),
            xgen(subf   , 8),
            xgen(divf   , 8),
        };

        #undef  xgen

        enum subset_t{
            hlt,    // halt
            bkp,    // break point
        };

        enum specail_t {
            tsc,    // time stamp
        };

        using u128 = __uint128_t;
        using i128 = __int128_t;

        struct cpu_t{
        private:
            using the_t = cpu_t;

            enum{
                integer_register_count  = 16,
                floating_register_count = 16,
            };

            voidp mem;
            uxx   bytes;
            opc_t ins;

            struct state_t{
                struct {
                    inc::bitbind<u16> operator[](uxx index){
                        return { & mask, index };
                    }
                private:
                    u16 mask = 0;
                } is_f64;

                union{
                    u16     zf  : 1;    // zero flag
                    struct {
                        u16 eq  : 1;    // equal
                        u16 cf  : 1;    // carry flag
                        u16 ab  : 1;    // above
                        u16 gt  : 1;    // great than
                        u16 ov  : 1;    // overflow
                    };
                };

                operator u64 & (){
                    return u64p(this)[0];
                }

                void operator =(u64 value){
                    operator u64 &() = value;
                }
            private:
                u32 rsv;
            } sta;

            struct imm_t{
                using the_t = imm_t;

                uxx width(){
                    return bits;
                }

                imm_t & load(u64 value, uxx width){
                    v    |= value << bits;
                    bits += width;

                    if (bits > 64){
                        bits = 64;
                    }
                    return the;
                }

                imm_t & clear(){
                    operator u64();
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
                
                operator f64(){
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
                i08 ib;
                i16 iw;
                i32 id;
                i64 i;

                u08 ub;
                u16 uw;
                u32 ud;
                u64 u;

                f32 fs;
                f64 fd;

                reg_t() : u(0){}
                reg_t(u64 v) : u(v){}
                reg_t(i64 v) : i(v){}

            }     r[integer_register_count], 
                  f[floating_register_count],
                  pc,               // program counter
                  csp,              // call stack pointer
                  tbp,              // temp stack base pointer
                & sp = r[ins.sp],   // stack pointer
                & bp = r[ins.bp],   // stack base pointer
                & t  = r[ins.t],    // temp
                & x  = r[ins.x];    // extern

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

            template<class imm_t, class type, class lambda>
            void opcx(type * r, uxx mask, lambda && op){
                switch(opcx_t(ins.op & mask)){
                case aai:   op(r[ins.a], r[ins.a], imm_t(imm.load(ins.im4, 4))); break;
                case aab:   op(r[ins.a], r[ins.a], r[ins.b]);                    break;
                case abt:   op(r[ins.a], r[ins.b], r[ins.t]);                    break;
                case tab:   op(r[ins.t], r[ins.a], r[ins.b]);                    break;
                case tai:   op(r[ins.t], r[ins.a], imm_t(imm.load(ins.im4, 4))); break;
                case abi:   op(r[ins.a], r[ins.a], imm_t(imm));                  break;
                case aib:   op(r[ins.a], imm_t(imm), r[ins.a]);                  break;
                case atb:   op(r[ins.a], r[ins.t], r[ins.b]);                    break;
                }
            }

            template<class imm_t, class type, class lambda>
            void opcx(type * r, lambda && op){
                opcx<imm_t>(r, 0x7, op);
            }

            template<class imm_t, class type, class lambda>
            void opiv(type * r, lambda && op){
                opcx<imm_t>(r, 0x3, op);
            }

            enum{
                with_cf = 1,
            };

            void builtin_sub(reg_t & a, reg_t b, reg_t c, uxx with_cf){
                u128 u = u128(b.u) - c.u - (sta.cf & with_cf);
                a.u    = u64(u);
                sta.cf = u64(u >> 64) & 1;
                sta.zf = a.u == 0;
                sta.ab = b.u > c.u;
                sta.gt = b.i > c.i;
            }

            void builtin_add(reg_t & a, reg_t b, reg_t c, uxx with_cf){
                u128 u = u128(b.u) + c.u + (sta.cf & with_cf);
                a.u    = u64(u);
                sta.cf = u64(u >> 64) & 1;
                sta.zf = a.u == 0;
            }

            reg_t builtin_mul(reg_t & a, reg_t b, reg_t c){
                u128 u = i128(b.i) * c.i;
                i64  h = i64(u >> 64);
                a.i    = i64(u);
                sta.zf = a.i == 0;
                sta.ov = h != 0 and h != -1;
                return h;
            }

            reg_t builtin_mulu(reg_t & a, reg_t b, reg_t c){
                u128 u = u128(b.i) * c.u;
                u64  h = u64(u >> 64);
                a.u    = u64(u);
                sta.zf = a.i == 0;
                sta.ov = h != 0;
                return h;
            }

            void builtin_div(reg_t & a, reg_t b, reg_t c){
                if (sta.ov = (c.u == 0); sta.ov){
                    return;
                }

                a.i    = b.i / c.i;
                x.i    = b.i % c.i;
                sta.zf = a.i == 0;
                sta.cf = x.i == 0;
            }

            void builtin_divu(reg_t & a, reg_t b, reg_t c){
                if (sta.ov = (c.u == 0); sta.ov){
                    return;
                }

                a.u    = b.u / c.u;
                x.u    = b.u % c.u;
                sta.zf = a.u == 0;
                sta.cf = x.u == 0;
            }

            void builtin_pop(reg_t * ptr){
                uxx  mask = ins.ab;
                bool need_load_sta = false;

                if (ins.op & 1){ // 下一组，每组 8 个寄存器
                    ptr  += 8;
                    static_assert(integer_register_count == 16);
                    static_assert(floating_register_count == 16);
                }

                while(true){
                    if (auto i = inc::index_of_last_set(mask); i == not_exist){
                        break;
                    }
                    else{
                        ptr[i].u = read<u64>(sp.u -= sizeof(u64), 0);
                        mask ^= uxx(1) << i;
                    }
                }
            }

            enum{
                is_start_push = 1,
            };

            void builtin_psh(reg_t * ptr, bool is_start_push){
                uxx mask  = ins.ab;

                if (is_start_push){
                    tbp   = bp;
                    bp    = sp;
                }
                if (ins.op & 1){ // 下一组，每组 8 个寄存器
                    ptr  += 8;
                    static_assert(integer_register_count == 16);
                    static_assert(floating_register_count == 16);
                }

                while(true){
                    if (auto i = inc::index_of_first_set(mask); i == not_exist){
                        break;
                    }
                    else{
                        write<u64>(ptr[i].u, sp.u, 0);
                        sp.u += sizeof(u64);
                        mask ^= uxx(1) << i;
                    }
                }
            }

            void builtin_if(bool condition){
                if (i64 offset = imm.load(ins.ab, 8); condition == false){
                    pc.i += offset * 2; // 每条指令 2 字节
                }
            }

            void builtin_csp_psh(){
                write<u64>(pc.u, csp.u, 0); 
                write<u64>(tbp.u, csp.u += sizeof(u64), 0);
                csp.u += sizeof(u64);
            }

            void builtin_csp_pop(){
                sp   = bp;
                bp.u = read<u64>(csp.u -= sizeof(u64), 0);
                pc.u = read<u64>(csp.u -= sizeof(u64), 0);
            }

            template<class type>
            type read(u64 base, i64 offset){
                using ptr_t = type *;
                return ptr_t(u08p(mem) + base + offset)[0];
            }

            template<class type>
            void write(type src, u64 base, i64 offset){
                using ptr_t = type *;
                ptr_t(u08p(mem) + base + offset)[0] = src;
            }

            enum{
                code_start = 0,
            };

        public:
            void run(){
                while(true){
                    ins   = read<opc_t>(pc.u, 0);
                    pc.u += 2; // 每条指令 2 字节
                    sets[ins.op](this);
                }
            }

            void set(area_t size){
                pc.u    = code_start;
                csp.u   = pc.u + size.code();
                sp.u    = csp.u + size.call_stack();
                bp.u    = sp.u;
            }

            void code(uxx index, opc_t value){
                write<opc_t>(value, code_start, index);
            }

            cpu_t(voidp mem, uxx bytes) : 
                mem(mem), bytes(bytes) {

                #define xgen(cmd)   for(uxx __i = cmd; __i <= cmd ## _end; __i++) sets[__i] = [this]()
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

                xgen(ldi){
                    imm.load(ins & 0xfff, 12);
                };

                xgen(shl){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        xgen2(64) {
                            x.u    = b.u >> (64 - c.u);
                            a.u    = b.u << c.u;
                            sta.zf = a.u == 0;
                            sta.cf = x.u & 1;
                        }
                    });
                };

                xgen(shr){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        xgen2(64) {
                            x.u    = b.u >> (c.u - 1);
                            a.u    = b.u << c.u;
                            sta.zf = a.u == 0;
                            sta.cf = x.u & 1;
                        }
                    });
                };

                xgen(sar){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        xgen2(63){
                            x.u    = b.i >> (c.u - 1);
                            a.u    = b.i << c.u;
                            sta.zf = a.u == 0;
                            sta.cf = x.u & 1;
                        }
                    });
                };

                #undef  xgen2

                xgen(bts){

                };

                xgen(ldib){
                    r[ins.a].u = read<i08>(r[ins.b].u, imm);
                };

                xgen(ldiw){
                    r[ins.a].u = read<i16>(r[ins.b].u, imm);
                };

                xgen(ldid){
                    r[ins.a].u = read<i32>(r[ins.b].u, imm);
                };

                xgen(ldiq){
                    r[ins.a].u = read<i64>(r[ins.b].u, imm);
                };

                xgen(ldub){
                    r[ins.a].u = read<u08>(r[ins.b].u, imm);
                };

                xgen(lduw){
                    r[ins.a].u = read<u16>(r[ins.b].u, imm);
                };

                xgen(ldud){
                    r[ins.a].u = read<u32>(r[ins.b].u, imm);
                };

                xgen(lduq){
                    r[ins.a].u = read<u64>(r[ins.b].u, imm);
                };

                xgen(ldtib){
                    r[ins.a].u = read<i08>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtiw){
                    r[ins.a].u = read<i16>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtid){
                    r[ins.a].u = read<i32>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtiq){
                    r[ins.a].u = read<i64>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtub){
                    r[ins.a].u = read<u08>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtuw){
                    r[ins.a].u = read<u16>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtud){
                    r[ins.a].u = read<u32>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldtuq){
                    r[ins.a].u = read<u64>(r[ins.b].u, t.i + i64(imm));
                };

                xgen(stb){
                    write<u08>(r[ins.a].ub, r[ins.b].u, i64(imm));
                };

                xgen(stw){
                    write<u16>(r[ins.a].uw, r[ins.b].u, i64(imm));
                };

                xgen(std){
                    write<u32>(r[ins.a].ud, r[ins.b].u, i64(imm));
                };

                xgen(stq){
                    write<u64>(r[ins.a].u, r[ins.b].u, i64(imm));
                };

                xgen(sttb){
                    write<u08>(r[ins.a].ub, r[ins.b].u, t.i + i64(imm));
                };

                xgen(sttw){
                    write<u16>(r[ins.a].uw, r[ins.b].u, t.i + i64(imm));
                };

                xgen(sttd){
                    write<u32>(r[ins.a].ud, r[ins.b].u, t.i + i64(imm));
                };

                xgen(sttq){
                    write<u64>(r[ins.a].u, r[ins.b].u, t.i + i64(imm));
                };

                xgen(ldfs){
                    f[ins.a].fs       = read<f32>(r[ins.b].u, imm);
                    sta.is_f64[ins.a] = false;
                };

                xgen(ldfd){
                    f[ins.a].fd       = read<f64>(r[ins.b].u, imm);
                    sta.is_f64[ins.a] = true;
                };

                xgen(ldtfs){
                    f[ins.a].fs       = read<f32>(r[ins.b].u, t.i + i64(imm));
                    sta.is_f64[ins.a] = false;
                };

                xgen(ldtfd){
                    f[ins.a].fs       = read<f64>(r[ins.b].u, t.i + i64(imm));
                    sta.is_f64[ins.a] = true;
                };

                xgen(stfs){
                    write<f32>(f[ins.a].fs, r[ins.b].u, imm);
                };

                xgen(stfd){
                    write<f64>(f[ins.a].fd, r[ins.b].u, imm);
                };

                xgen(sttfs){
                    write<f32>(f[ins.a].fs, r[ins.b].u, t.i + i64(imm));
                };

                xgen(sttfd){
                    write<f64>(f[ins.a].fd, r[ins.b].u, t.i + i64(imm));
                };

                xgen(pop){
                    builtin_pop(r);
                };

                xgen(popf){
                    builtin_pop(r);
                };

                xgen(psh){
                    builtin_psh(r, not is_start_push);
                };

                xgen(pshs){
                    builtin_psh(r, is_start_push);
                };

                xgen(pshf){
                    builtin_psh(f, not is_start_push);
                };

                xgen(pshfs){
                    builtin_psh(f, is_start_push);
                };

                xgen(bdc){
                    auto mask = ins.a;
                    auto ptr  = r + (ins.op & 0x3) * 4;

                    for(; mask != 0; ptr++, mask >>= 1){
                        if (mask & 1){
                            ptr[0] = r[ins.b];
                        }
                    }
                };

                xgen(movbx){
                    r[ins.a].u = r[ins.b].ib;
                };

                xgen(movwx){
                    r[ins.a].u = r[ins.b].iw;
                };

                xgen(movdx){
                    r[ins.a].u = r[ins.b].id;
                };

                xgen(movix){
                    r[ins.a].i = imm.load(ins.im4, 4);
                };

                xgen(movb){
                    r[ins.a].u = r[ins.b].ub;
                };

                xgen(movw){
                    r[ins.a].u = r[ins.b].uw;
                };

                xgen(movd){
                    r[ins.a].u = r[ins.b].ud;
                };

                xgen(movq){
                    r[ins.a].u = r[ins.b].u;
                };

                xgen(movfsi){
                    f[ins.a].fs = imm.load(ins.im4, 4);
                };

                xgen(movfs){
                    f[ins.a].fs = f[ins.b].fs;
                };

                xgen(movfdi){
                    f[ins.a].fd = imm.load(ins.im4, 4);
                };

                xgen(movfd){
                    f[ins.a].fd = f[ins.b].fd;
                };

                xgen(uq2fs){
                    f[ins.a].fs = r[ins.b].u;
                };

                xgen(uq2fd){
                    f[ins.a].fd = r[ins.b].u;
                };

                xgen(iq2fs){
                    f[ins.a].fs = r[ins.b].i;
                };

                xgen(iq2fd){
                    f[ins.a].fd = r[ins.b].i;
                };

                xgen(add){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_add(a, b, c, not with_cf);
                    });
                };

                xgen(adc){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_add(a, b, c, with_cf);
                    });
                };

                xgen(sbb){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_sub(a, b, c, with_cf);
                    });
                };

                xgen(sub){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_sub(a, b, c, not with_cf);
                    });
                };

                xgen(mul){
                    opcx<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_mul(a, b, c);
                    });
                };

                xgen(mulu){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_mulu(a, b, c);
                    });
                };

                xgen(mulx){
                    opcx<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        x = builtin_mul(a, b, c);
                    });
                };

                xgen(mulxu){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        x = builtin_mulu(a, b, c);
                    });
                };

                xgen(div){
                    opcx<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_div(a, b, c);
                    });
                };

                xgen(divu){
                    opcx<u64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        builtin_divu(a, b, c);
                    });
                };

                xgen(andi){
                    opiv<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        a.u    = b.u & c.u;
                        sta.zf = a.u == 0;
                    });
                };

                xgen(ori){
                    opiv<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        a.u    = b.u | c.u;
                        sta.zf = a.u == 0;
                    });
                };

                xgen(nandi){
                    opiv<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        a.u    = b.u & c.u;
                        a.u    = ~a.u;
                        sta.zf = a.u == 0;
                    });
                };

                xgen(xori){
                    opiv<i64>(r, [this](reg_t & a, reg_t b, reg_t c){
                        a.u    = b.u ^ c.u;
                        sta.zf = a.u == 0;
                    });
                };

                xgen(mask){
                    u64 mask   = u64(1) << r[ins.b].u;
                    r[ins.a].u = r[ins.a].u & (mask - 1);
                    sta.zf     = r[ins.a].u == 0;
                };

                xgen(ldbs){
                    bit_range_t range = u64(imm);
                    u64 mask = (u64(1) << (range.last_index + 1)) - 1;
                    u64 bits = (r[ins.b].u & mask);
                    bits     = (bits >> range.first_index);
                };

                xgen(stbs){
                    bit_range_t range = u64(imm);
                    u64 mask    = (u64(1) << (range.last_index - range.first_index + 1)) - 1;
                    u64 v       = (r[ins.a].u & mask) << range.first_index;
                    mask      <<= range.first_index;
                    r[ins.b].u  = r[ins.b].u & u64(~mask);
                    r[ins.b].u |= v;
                };

                xgen(sbc){

                };

                xgen(ios){
                    r[ins.a].u = inc::index_of_first_set(r[ins.b].u);
                    sta.zf     = r[ins.b].u == 0;
                };

                xgen(lis){
                    r[ins.a].u = inc::index_of_last_set(r[ins.b].u);
                    sta.zf     = r[ins.b].u == 0;
                };

                // xgen(___){

                // };

                xgen(swp){
                    inc::swap(& r[ins.a], & r[ins.b]);
                };

                xgen(swpf){
                    inc::swap(& f[ins.a], & f[ins.b]);
                    sta.is_f64[ins.a].swap_value(sta.is_f64[ins.b]);
                };

                xgen(lock){
                    // TODO:===================================================================
                };

                xgen(ifeq){
                    builtin_if(sta.eq);
                };

                xgen(ifne){
                    builtin_if(not sta.eq);
                };

                xgen(ifcf){
                    builtin_if(sta.cf);
                };

                xgen(ifnc){
                    builtin_if(not sta.cf);
                };

                xgen(ifab){
                    builtin_if(sta.ab);
                };

                xgen(ifae){
                    builtin_if(sta.ab or sta.eq);
                };

                xgen(ifbt){
                    builtin_if(not (sta.ab or sta.eq));
                };

                xgen(ifbe){
                    builtin_if(not sta.ab);
                };

                xgen(ifgt){
                    builtin_if(sta.gt);
                };

                xgen(ifge){
                    builtin_if(sta.gt or sta.eq);
                };

                xgen(iflt){
                    builtin_if(not (sta.gt or sta.eq));
                };

                xgen(ifle){
                    builtin_if(not sta.gt);
                };

                xgen(ifov){
                    builtin_if(sta.ov);
                };

                xgen(ifno){
                    builtin_if(not sta.ov);
                };

                xgen(jmp){
                    builtin_if(true);
                };

                xgen(ret){
                    builtin_csp_pop();
                };

                xgen(call){
                    builtin_csp_psh();
                    pc.u      = r[ins.a].u;
                };

                xgen(calli){
                    builtin_csp_psh();
                    uxx width = imm.load(ins.ab, 8).width();
                    u64 mask  = u64(1) << width;
                    pc.u     &= mask - 1;
                    pc.u     |= u64(imm);
                };

                xgen(addf){
                    // TODO:===================================================================
                };

                xgen(mulf){
                    // TODO:===================================================================
                };

                xgen(subf){
                    // TODO:===================================================================
                };

                xgen(divf){
                    // TODO:===================================================================
                };

                #undef  xgen
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::draft_isa::area_t;
    using ::mixc::draft_isa::bit_range_t;
    using ::mixc::draft_isa::cpu_t;
    using ::mixc::draft_isa::opc_t;

    using ::mixc::draft_isa::aai;
    using ::mixc::draft_isa::aab;
    using ::mixc::draft_isa::abt;
    using ::mixc::draft_isa::tab;
    using ::mixc::draft_isa::tai;
    using ::mixc::draft_isa::abi;
    using ::mixc::draft_isa::aib;
    using ::mixc::draft_isa::atb;

    enum{
        sp  = opc_t::sp,
        bp  = opc_t::bp,
        rt  = opc_t::t,
        rx  = opc_t::x,
        r4, r5, r6, r7, r8, r9, ra, rb, rc, rd, re, rf
    };

    namespace iss{ // instruction set
        using ::mixc::draft_isa::ldi;
        using ::mixc::draft_isa::shl;
        using ::mixc::draft_isa::shr;
        using ::mixc::draft_isa::sar;
        using ::mixc::draft_isa::bts;
        using ::mixc::draft_isa::ldib;
        using ::mixc::draft_isa::ldiw;
        using ::mixc::draft_isa::ldid;
        using ::mixc::draft_isa::ldiq;
        using ::mixc::draft_isa::ldub;
        using ::mixc::draft_isa::lduw;
        using ::mixc::draft_isa::ldud;
        using ::mixc::draft_isa::lduq;
        using ::mixc::draft_isa::ldtib;
        using ::mixc::draft_isa::ldtiw;
        using ::mixc::draft_isa::ldtid;
        using ::mixc::draft_isa::ldtiq;
        using ::mixc::draft_isa::ldtub;
        using ::mixc::draft_isa::ldtuw;
        using ::mixc::draft_isa::ldtud;
        using ::mixc::draft_isa::ldtuq;
        using ::mixc::draft_isa::stb;
        using ::mixc::draft_isa::stw;
        using ::mixc::draft_isa::std;
        using ::mixc::draft_isa::stq;
        using ::mixc::draft_isa::sttb;
        using ::mixc::draft_isa::sttw;
        using ::mixc::draft_isa::sttd;
        using ::mixc::draft_isa::sttq;
        using ::mixc::draft_isa::ldfs;
        using ::mixc::draft_isa::ldfd;
        using ::mixc::draft_isa::ldtfs;
        using ::mixc::draft_isa::ldtfd;
        using ::mixc::draft_isa::stfs;
        using ::mixc::draft_isa::stfd;
        using ::mixc::draft_isa::sttfs;
        using ::mixc::draft_isa::sttfd;
        using ::mixc::draft_isa::pop;
        using ::mixc::draft_isa::popf;
        using ::mixc::draft_isa::psh;
        using ::mixc::draft_isa::pshs;
        using ::mixc::draft_isa::pshf;
        using ::mixc::draft_isa::pshfs;
        using ::mixc::draft_isa::bdc;
        using ::mixc::draft_isa::movbx;
        using ::mixc::draft_isa::movwx;
        using ::mixc::draft_isa::movdx;
        using ::mixc::draft_isa::movix;
        using ::mixc::draft_isa::movb;
        using ::mixc::draft_isa::movw;
        using ::mixc::draft_isa::movd;
        using ::mixc::draft_isa::movq;
        using ::mixc::draft_isa::movfsi;
        using ::mixc::draft_isa::movfs;
        using ::mixc::draft_isa::movfdi;
        using ::mixc::draft_isa::movfd;
        using ::mixc::draft_isa::uq2fs;
        using ::mixc::draft_isa::uq2fd;
        using ::mixc::draft_isa::iq2fs;
        using ::mixc::draft_isa::iq2fd;
        using ::mixc::draft_isa::add;
        using ::mixc::draft_isa::adc;
        using ::mixc::draft_isa::sbb;
        using ::mixc::draft_isa::sub;
        using ::mixc::draft_isa::mul;
        using ::mixc::draft_isa::mulu;
        using ::mixc::draft_isa::mulx;
        using ::mixc::draft_isa::mulxu;
        using ::mixc::draft_isa::div;
        using ::mixc::draft_isa::divu;
        using ::mixc::draft_isa::andi;
        using ::mixc::draft_isa::ori;
        using ::mixc::draft_isa::nandi;
        using ::mixc::draft_isa::xori;
        using ::mixc::draft_isa::mask;
        using ::mixc::draft_isa::ldbs;
        using ::mixc::draft_isa::stbs;
        using ::mixc::draft_isa::sbc;
        using ::mixc::draft_isa::ios;
        using ::mixc::draft_isa::lis;
        using ::mixc::draft_isa::swp;
        using ::mixc::draft_isa::swpf;
        using ::mixc::draft_isa::lock;
        using ::mixc::draft_isa::ifeq;
        using ::mixc::draft_isa::ifne;
        using ::mixc::draft_isa::ifcf;
        using ::mixc::draft_isa::ifnc;
        using ::mixc::draft_isa::ifab;
        using ::mixc::draft_isa::ifae;
        using ::mixc::draft_isa::ifbt;
        using ::mixc::draft_isa::ifbe;
        using ::mixc::draft_isa::ifgt;
        using ::mixc::draft_isa::ifge;
        using ::mixc::draft_isa::iflt;
        using ::mixc::draft_isa::ifle;
        using ::mixc::draft_isa::ifov;
        using ::mixc::draft_isa::ifno;
        using ::mixc::draft_isa::jmp;
        using ::mixc::draft_isa::ret;
        using ::mixc::draft_isa::call;
        using ::mixc::draft_isa::calli;
        using ::mixc::draft_isa::addf;
        using ::mixc::draft_isa::mulf;
        using ::mixc::draft_isa::subf;
        using ::mixc::draft_isa::divf;
    }

    using ::mixc::draft_isa::set_t;
}

/*
等效指令            |   实际指令
------------------------------------------
store   bp          ;   pushs   rx, ...
mov     bp, sp      ;   
push    ...         ;   

call    func_name   ;   call    func_name
mov     sp, bp      ;   
load    bp          ;   
*/
