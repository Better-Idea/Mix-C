#pragma once
#include"definition/mini.hpp"
#include<x86intrin.h>
#include<atomic>

xspace(mixc)
    xspace(instruction)
        #define xgen(type)                                          \
        xdef(bit_test, type value, uxx index)                       \
            return (value & (type(1) << index)) != 0;               \
        $                                                           \
        xdef(bit_test_and_reverse, type * value, uxx index)         \
            type mask   = (type(1) << index);                       \
            bool result = (value[0] & mask) != 0;                   \
            value[0]   ^= mask;                                     \
            return result;                                          \
        $                                                           \
        xdef(bit_test_and_set, type * value, uxx index)             \
            type mask   = (type(1) << index);                       \
            bool result = (value[0] & mask) != 0;                   \
            value[0]   |= mask;                                     \
            return result;                                          \
        $                                                           \
        xdef(bit_test_and_reset, type * value, uxx index)           \
            type mask   = (type(1) << index);                       \
            bool result = (value[0] & mask) != 0;                   \
            value[0]   &= ~mask;                                    \
            return result;                                          \
        $

        xgen(u64);
        xgen(u32);
        xgen(u16);
        xgen(u08);
        #undef xgen

        #define xgen(w,f)                                           \
        xdef(count_of_set, u ## w value)                            \
            return (uxx)_popcnt ## w(value);                        \
        $                                                           \
        xdef(count_of_reset, u ## w value)                          \
            return (uxx)count_of_set(~value);                       \
        $                                                           \
        xdef(index_of_set, u ## w value)                            \
            xif(value == 0)                                         \
                return not_exist;                                   \
            $                                                       \
            return (uxx)__bsf ## f(value);                          \
        $                                                           \
        xdef(last_index_of_set, u ## w value)                       \
            xif(value == 0)                                         \
                return not_exist;                                   \
            $                                                       \
            return (uxx)__bsr ## f(value);                          \
        $

        xgen(32, d);

        #ifdef xsys64
            xgen(64, q);
        #elif defined xsys32
            xdef(count_of_set, u64 value) 
                return count_of_set(u32(value)) + count_of_set(u32(value >> 32));
            $

            xdef(count_of_reset, u64 value) 
                return count_of_set(~value);
            $

            xdef(index_of_set, u64 value)
                xif(u32(value))
                    return index_of_set(u32(value));
                $
                
                xif(u32(value >> 32))
                    return index_of_set(u32(value >> 32)) + 32;
                xes 
                    return not_exist;
                $
            $

            xdef(last_index_of_set, u64 value)
                xif(u32(value >> 32))
                    return index_of_set(u32(value)) + 32;
                $
                
                xif(u32(value))
                    return index_of_set(u32(value));
                xes 
                    return not_exist;
                $
            $
        #endif
        #undef xgen

        #define xgen(x,w)                                           \
        xdef(trailing_zero, u ## w value)                           \
            return __builtin_ia32_lzcnt_u ## w(value);              \
        $                                                           \
        xge2(x,w)

        #define xge2(x,w)                                           \
        xdef(leading_zero, u ## w value)                            \
            return sizeof(value) * 8 - 1 -                          \
                last_index_of_set((x)value);                        \
        $                                                           \
        xdef(trailing_one, u ## w value)                            \
            return trailing_zero((u ## w)~value);                   \
        $                                                           \
        xdef(leading_one, u ## w value)                             \
            return leading_zero((u ## w)~value);                    \
        $

        xgen(u32, 16);
        xgen(u32, 32);

        #ifdef xsys64
            xgen(u64, 64);
        #elif defined xsys32
            xdef(trailing_zero, u64 value) 
                auto c = trailing_zero(u32(value));
                xif(u32(value))
                    return c;
                $
                return trailing_zero(u32(value >> 32)) + c;
            $
            xge2(u64, 64);
        #endif
        #undef xgen
        #undef xge2

        #define xgen(w,f)                                           \
        xdef(ring_shift_left, u ## w shifted, uxx shift)            \
            return (u ## w)__ror ## f(shifted, shift);              \
        $                                                           \
        xdef(ring_shift_right, u ## w shifted, uxx shift)           \
            return (u ## w)__rol ## f(shifted, shift);              \
        $

        xgen(08, b);
        xgen(16, w);
        xgen(32, d);

        #ifdef xsys64
            xgen(64, q);
        #elif defined xsys32
            xdef(ring_shift_left, u64 shifted, uxx shift) 
                shift &= 64 - 1;
                u64 l  = shift >> (64 - shift);
                u64 h  = shift << shift;
                return h | l;
            $

            xdef(ring_shift_right, u64 shifted, uxx shift) 
                shift &= 64 - 1;
                u64 l  = shift >> shift;
                u64 h  = shift << (64 - shift);
                return h | l;
            $
        #endif
        #undef xgen

        xdef(time_stamp) 
            return __rdtsc();
        $

        xdef(div, u64 * low, u64 a, u64 b) 
            low[0] = a / b;
            return a % b;
        $

        xdef(div, i64 * low, i64 a, i64 b) 
            low[0] = a / b;
            return a % b;
        $

        #ifdef xsys64
            xdef(add, u64 * low, u64 a, u64 b, u64 cf = 0) 
                return (u64)_addcarry_u64(u08(cf), a, b, low);
            $

            xdef(sub, u64 * low, u64 a, u64 b, u64 cf = 0) 
                return (u64)_subborrow_u64(u08(cf), a, b, low);
            $

            xdef(mul, u64 * low, u64 a, u64 b) 
                u64 high;
                low[0] = _mulx_u64(a, b, & high);
                return high;
            $

            xdef(mul, i64 * low, i64 a, i64 b) 
                i64 high;
                asm(""::"a"(a),"d"(b));
                asm("imul %%rdx":"=d"(high), "=a"(low[0]));
                return high;
            $

        #elif defined xsys32
            xdef(add, u64 * low, u64 a, u64 b, u64 cf = 0) 
                u32 ua [] =  u32(a), u32(a >> 32) $
                u32 ub [] =  u32(b), u32(b >> 32) $
                u32 uc [2];
                cf = _addcarry_u32(u08(cf), a[0], b[0], & c[0]);
                cf = _addcarry_u32(u08(cf), a[1], b[1], & c[1]);
                low[0] = u64(uc[1]) << 32 | uc[0];
                return cf;
            $

            xdef(sub, u64 * low, u64 a, u64 b, u64 cf = 0) 
                u32 ua [] =  u32(a), u32(a >> 32) $
                u32 ub [] =  u32(b), u32(b >> 32) $
                u32 uc [2];
                cf = _subborrow_u32(u08(cf), a[0], b[0], & c[0]);
                cf = _subborrow_u32(u08(cf), a[1], b[1], & c[1]);
                low[0] = u64(uc[1]) << 32 | uc[0];
                return cf;
            $
            
            xdef(mul, u64 * low, u64 a, u64 b) 
                u64 high;
                u32 l [] =  u32(a), u32(a >> 32), 0u, 0u $
                u32 h [] =  u32(b), u32(b >> 32), 0u, 0u $
                
                u32 ua = l[0];
                u32 ub = l[1];
                u32 uc = h[0];
                u32 ud = h[1];
                u08 cf = 0;

                h[0] = _mulx_u32(a, c, & l[0]);
                h[1] = _mulx_u32(a, d, & l[1]);
                h[2] = _mulx_u32(b, c, & l[2]);
                h[3] = _mulx_u32(b, d, & l[3]);

                cf = _addcarry_u32(cf, h[0], l[1], l + 1);
                cf = _addcarry_u32(cf, l[1], l[2], l + 1);
                cf = _addcarry_u32(cf, h[1], h[2], h + 2);
                cf = _addcarry_u32(cf, h[2], l[3], h + 2);
                h[3] += cf;

                low[0] = u64(l[1]) << 32 | l[0];
                high = u64(H[1]) << 32 | H[0];
                return high;
            $
            
            xdef(mul, i64 * low, i64 a, i64 b) 
                u64 l;
                u64 h;
                u08 cf = 0;
                i64 high;
                h = mul(& l, u64(a), u64(b));

                xif(bit_test(u64(a), 63)) 
                    xif(bit_test(u64(b), 63))  //负负得正
                        cf = sub(& l, cf, l);
                        cf = sub(& h, cf, h);
                    xes
                        h |= u64(-1) << last_index_of_set(h);
                    $
                xes
                    xif(bit_test(u64(b), 63)) 
                        h |= u64(-1) << last_index_of_set(h);
                    $
                $

                low = i64(L);
                high = i64(H);
                return high;
            $
        #endif
    $

    xspace(instruction)
        #define xgen(type)                                          \
        xdef(atom_inc, type * a)                                    \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            temp++;                                                 \
        $                                                           \
        xdef(atom_dec, type * a)                                    \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            temp--;                                                 \
        $                                                           \
        xdef(atom_add, type * a, type b)                            \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            temp += b;                                              \
        $                                                           \
        xdef(atom_sub, type * a, type b)                            \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            temp -= b;                                              \
        $                                                           \
        xdef(atom_swap, type * a, type * b)                         \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            return (temp.exchange(*b));                             \
        $                                                           \
        xdef(atom_and, type * a, type b)                            \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            return (temp &= b);                                     \
        $                                                           \
        xdef(atom_or, type * a, type b)                             \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            return(temp |= b);                                      \
        $                                                           \
        xdef(atom_xor, type * a, type b)                            \
            using namespace std;                                    \
            atomic<type> & temp = *(atomic<type> *)(a);             \
            return(temp ^= b);                                      \
        $                                                                       

        xgen(u08);
        xgen(u16);
        xgen(u32);
        xgen(u64);

        xgen(i08);
        xgen(i16);
        xgen(i32);
        xgen(i64);
        #undef xgen
    $
$
