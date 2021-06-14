#ifndef xpack_algo_md5
#define xpack_algo_md5
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_md5::inc
#include"docker/array.hpp"
#include"instruction/ring_shift_left.hpp"
#include"interface/seqptr.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xstruct.hpp"
#include"memop/bytes_load.hpp"
#include"memop/bytes_store.hpp"
#include"memop/copy.hpp"
#include"memop/zeros.hpp"
#include"memop/is_big_endian.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_md5{
    inline auto F(u32 x, u32 y, u32 z){
        return (x & y) | (z & (~x));
    }

    inline auto G(u32 x, u32 y, u32 z){
        return (x & z) | (y & (~z));
    }

    inline auto H(u32 x, u32 y, u32 z){
        return (x ^ y ^ z);
    }

    inline auto I(u32 x, u32 y, u32 z){
        return (y ^ (x | (~z)));
    }

    inline auto FF(u32p ap, u32 b, u32 c, u32 d, u32 Mj, u32 s, u32 ti){
        *ap         = b + inc::ring_shift_left(*ap + F(b, c, d) + Mj + ti, s);
    }

    inline auto GG(u32p ap, u32 b, u32 c, u32 d, u32 Mj, u32 s, u32 ti){
        *ap         = b + inc::ring_shift_left(*ap + G(b, c, d) + Mj + ti, s);
    }

    inline auto HH(u32p ap, u32 b, u32 c, u32 d, u32 Mj, u32 s, u32 ti){
        *ap         = b + inc::ring_shift_left(*ap + H(b, c, d) + Mj + ti, s);
    }

    inline auto II(u32p ap, u32 b, u32 c, u32 d, u32 Mj, u32 s, u32 ti){
        *ap         = b + inc::ring_shift_left(*ap + I(b, c, d) + Mj + ti, s);
    }

    inline auto FFF(u32p ap, u32p bp, u32p cp, u32p dp, cu32p M){
        u32 & a     = *ap;
        u32 & b     = *bp;
        u32 & c     = *cp;
        u32 & d     = *dp;

        FF(xref(a), b, c, d, M[0 ], 7 , 0xd76aa478);
        FF(xref(d), a, b, c, M[1 ], 12, 0xe8c7b756);
        FF(xref(c), d, a, b, M[2 ], 17, 0x242070db);
        FF(xref(b), c, d, a, M[3 ], 22, 0xc1bdceee);
        FF(xref(a), b, c, d, M[4 ], 7 , 0xf57c0faf);
        FF(xref(d), a, b, c, M[5 ], 12, 0x4787c62a);
        FF(xref(c), d, a, b, M[6 ], 17, 0xa8304613);
        FF(xref(b), c, d, a, M[7 ], 22, 0xfd469501);
        FF(xref(a), b, c, d, M[8 ], 7 , 0x698098d8);
        FF(xref(d), a, b, c, M[9 ], 12, 0x8b44f7af);
        FF(xref(c), d, a, b, M[10], 17, 0xffff5bb1);
        FF(xref(b), c, d, a, M[11], 22, 0x895cd7be);
        FF(xref(a), b, c, d, M[12], 7 , 0x6b901122);
        FF(xref(d), a, b, c, M[13], 12, 0xfd987193);
        FF(xref(c), d, a, b, M[14], 17, 0xa679438e);
        FF(xref(b), c, d, a, M[15], 22, 0x49b40821);
    }

    inline auto GGG(u32p ap, u32p bp, u32p cp, u32p dp, cu32p M){
        u32 & a     = *ap;
        u32 & b     = *bp;
        u32 & c     = *cp;
        u32 & d     = *dp;

        GG(xref(a), b, c, d, M[1 ], 5 , 0xf61e2562);
        GG(xref(d), a, b, c, M[6 ], 9 , 0xc040b340);
        GG(xref(c), d, a, b, M[11], 14, 0x265e5a51);
        GG(xref(b), c, d, a, M[0 ], 20, 0xe9b6c7aa);
        GG(xref(a), b, c, d, M[5 ], 5 , 0xd62f105d);
        GG(xref(d), a, b, c, M[10], 9 , 0x02441453);
        GG(xref(c), d, a, b, M[15], 14, 0xd8a1e681);
        GG(xref(b), c, d, a, M[4 ], 20, 0xe7d3fbc8);
        GG(xref(a), b, c, d, M[9 ], 5 , 0x21e1cde6);
        GG(xref(d), a, b, c, M[14], 9 , 0xc33707d6);
        GG(xref(c), d, a, b, M[3 ], 14, 0xf4d50d87);
        GG(xref(b), c, d, a, M[8 ], 20, 0x455a14ed);
        GG(xref(a), b, c, d, M[13], 5 , 0xa9e3e905);
        GG(xref(d), a, b, c, M[2 ], 9 , 0xfcefa3f8);
        GG(xref(c), d, a, b, M[7 ], 14, 0x676f02d9);
        GG(xref(b), c, d, a, M[12], 20, 0x8d2a4c8a);
    }

    inline auto HHH(u32p ap, u32p bp, u32p cp, u32p dp, cu32p M){
        u32 & a     = *ap;
        u32 & b     = *bp;
        u32 & c     = *cp;
        u32 & d     = *dp;

        HH(xref(a), b, c, d, M[5 ], 4 , 0xfffa3942);
        HH(xref(d), a, b, c, M[8 ], 11, 0x8771f681);
        HH(xref(c), d, a, b, M[11], 16, 0x6d9d6122);
        HH(xref(b), c, d, a, M[14], 23, 0xfde5380c);
        HH(xref(a), b, c, d, M[1 ], 4 , 0xa4beea44);
        HH(xref(d), a, b, c, M[4 ], 11, 0x4bdecfa9);
        HH(xref(c), d, a, b, M[7 ], 16, 0xf6bb4b60);
        HH(xref(b), c, d, a, M[10], 23, 0xbebfbc70);
        HH(xref(a), b, c, d, M[13], 4 , 0x289b7ec6);
        HH(xref(d), a, b, c, M[0 ], 11, 0xeaa127fa);
        HH(xref(c), d, a, b, M[3 ], 16, 0xd4ef3085);
        HH(xref(b), c, d, a, M[6 ], 23, 0x04881d05);
        HH(xref(a), b, c, d, M[9 ], 4 , 0xd9d4d039);
        HH(xref(d), a, b, c, M[12], 11, 0xe6db99e5);
        HH(xref(c), d, a, b, M[15], 16, 0x1fa27cf8);
        HH(xref(b), c, d, a, M[2 ], 23, 0xc4ac5665);
    }

    inline auto III(u32p ap, u32p bp, u32p cp, u32p dp, cu32p M){
        u32 & a     = *ap;
        u32 & b     = *bp;
        u32 & c     = *cp;
        u32 & d     = *dp;

        II(xref(a), b, c, d, M[0 ], 6 , 0xf4292244);
        II(xref(d), a, b, c, M[7 ], 10, 0x432aff97);
        II(xref(c), d, a, b, M[14], 15, 0xab9423a7);
        II(xref(b), c, d, a, M[5 ], 21, 0xfc93a039);
        II(xref(a), b, c, d, M[12], 6 , 0x655b59c3);
        II(xref(d), a, b, c, M[3 ], 10, 0x8f0ccc92);
        II(xref(c), d, a, b, M[10], 15, 0xffeff47d);
        II(xref(b), c, d, a, M[1 ], 21, 0x85845dd1);
        II(xref(a), b, c, d, M[8 ], 6 , 0x6fa87e4f);
        II(xref(d), a, b, c, M[15], 10, 0xfe2ce6e0);
        II(xref(c), d, a, b, M[6 ], 15, 0xa3014314);
        II(xref(b), c, d, a, M[13], 21, 0x4e0811a1);
        II(xref(a), b, c, d, M[4 ], 6 , 0xf7537e82);
        II(xref(d), a, b, c, M[11], 10, 0xbd3af235);
        II(xref(c), d, a, b, M[2 ], 15, 0x2ad7d2bb);
        II(xref(b), c, d, a, M[9 ], 21, 0xeb86d391);
    }

    using v_t = u32[16];
}

namespace mixc::algo_md5::origin{
    xstruct(
        xname(md5_result),
        xpubb(inc::array<u08, 16>)
    )
        using inc::array<u08, 16>::array;
    $

    xstruct(
        xname(md5),
        xprif(m_v, v_t),
        xprif(m_i, uxx),
        xprif(m_a, u32),
        xprif(m_b, u32),
        xprif(m_c, u32),
        xprif(m_d, u32)
    )
        md5() : 
            m_v{ },
            m_i{ 0 },
            m_a{ 0x67452301 },
            m_b{ 0xefcdab89 },
            m_c{ 0x98badcfe },
            m_d{ 0x10325476 }{
        }

        template<inc::can_unified_seqlize seq_t>
        requires(sizeof(inc::item_origin_of<seq_t>) == 1)
        md5(seq_t const & source) : md5(){
            this->append(source);
        }

        md5(const void * source, uxx bytes): md5(){
            this->append(source, bytes);
        }

        md5 & reset(){
            *this               = md5{};
            return *this;
        }

        md5 & append(const void * source, uxx bytes){
            return this->append_core(inc::seqptr<u08>{u08p(source), bytes});
        }

        template<inc::can_unified_seqlize seq_t>
        requires(sizeof(inc::item_origin_of<seq_t>) == 1)
        md5 & append(seq_t const & source){
            return this->append_core(inc::unified_seq<seq_t>{source});
        }

        operator md5_result(){
            uxx  i_offset       = m_i & 0x3f;
            uxx  reserved       = 9;
            md5  r              = *this;
            u08p buffer         = u08p(r.m_v);

            inc::zeros(r.m_v, sizeof(r.m_v));

            // 至少存在一个可用字节用于存放 0x80
            buffer[i_offset]    = 0x80;

            if (inc::copy_unsafe(buffer, cu08p(m_v), i_offset); i_offset + reserved <= 64){
                inc::bytes_store<u64>(buffer + 56, m_i * 8);
                r.update();
            }
            else{
                r.update();
                inc::zeros(r.m_v, sizeof(r.m_v));
                inc::bytes_store<u64>(buffer + 56, m_i * 8);
                r.update();
            }
            return md5_result{ 
                u08((r.m_a >>  0) & 0xff), u08((r.m_a >>  8) & 0xff), u08((r.m_a >> 16) & 0xff), u08((r.m_a >> 24) & 0xff),
                u08((r.m_b >>  0) & 0xff), u08((r.m_b >>  8) & 0xff), u08((r.m_b >> 16) & 0xff), u08((r.m_b >> 24) & 0xff),
                u08((r.m_c >>  0) & 0xff), u08((r.m_c >>  8) & 0xff), u08((r.m_c >> 16) & 0xff), u08((r.m_c >> 24) & 0xff),
                u08((r.m_d >>  0) & 0xff), u08((r.m_d >>  8) & 0xff), u08((r.m_d >> 16) & 0xff), u08((r.m_d >> 24) & 0xff),
            };
        }
    private:
        template<class seq_t>
        md5 & append_core(seq_t && source){
            uxx i_offset        = m_i & 0x3f;
            uxx i_end           = i_offset + source.length();

            while(i_end >= 64){
                auto copy_size  = 64 - i_offset;
                inc::copy_unsafe(u08p(m_v) + i_offset, source, copy_size);
                source          = source.backward(copy_size);
                i_end          -= 64;
                i_offset        = 0;

                if (inc::is_big_endian()){
                    m_v[0]      = inc::bytes_load<u32>(m_v + 0);
                    m_v[1]      = inc::bytes_load<u32>(m_v + 1);
                    m_v[2]      = inc::bytes_load<u32>(m_v + 2);
                    m_v[3]      = inc::bytes_load<u32>(m_v + 3);
                }
                this->update();
            }

            if (i_end != 0){
                auto copy_size  = i_end - i_offset;
                inc::copy_unsafe(u08p(m_v) + i_offset, source, copy_size);
            }

            m_i                += source.length();
            return *this;
        }

        void update(){
            u32 ta              = m_a;
            u32 tb              = m_b;
            u32 tc              = m_c;
            u32 td              = m_d;

            FFF(xref(m_a), xref(m_b), xref(m_c), xref(m_d), m_v);
            GGG(xref(m_a), xref(m_b), xref(m_c), xref(m_d), m_v);
            HHH(xref(m_a), xref(m_b), xref(m_c), xref(m_d), m_v);
            III(xref(m_a), xref(m_b), xref(m_c), xref(m_d), m_v);

            m_a                += ta;
            m_b                += tb;
            m_c                += tc;
            m_d                += td;
        }
    $;
}

#endif

xexport_space(mixc::algo_md5::origin)
