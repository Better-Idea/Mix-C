#ifndef xpack_draft_mod
#define xpack_draft_mod
#pragma push_macro("xuser")
#undef  xuser   
#define xuser mixc::draft_mod::inc
#include"instruction/mod.hpp"
#include"instruction/mul.hpp"
#include"mixc.hpp"
#include"utils/mfxx.hpp"
#pragma pop_macro("xuser")

// 小猫的杂货屋
// 不具有实用性的取余操作
namespace mixc::draft_mod{
    f64 mod(f64 a, f64 b){
        using namespace inc;
        auto ma  = mf64(a);
        auto mb  = mf64(b);

        auto e0  = mb.real_exp_unsafe();
        auto e1  = ma.real_exp_unsafe() - e0;
        auto e2  = e1;
        auto r0  = e2 % 64;
        e2       = e2 / 64;
        ma.real_exp(ma.bits_of_mantissa() + r0);
        mb.real_exp(mb.bits_of_mantissa());
        auto da  = mul(ma.real_dec_unsafe(), 1ull << r0);
        auto db  = mb.real_dec_unsafe();
        auto m   = mod(da, db);
        auto s   = u128{}.l(m).h(0);
        auto x   = u128{}.l(0).h(1);

        for(; e2 != 0; x = mul(x.low, x.low), e2 >>= 1){
            if (x.l(mod(x, db)).h(0); e2 & 1){
                s = mul(s.low, x.low);
                s.l(mod(s, db)).h(0);
            }
        }

        auto r1  = mf64(s.low);
        auto e3  = r1.real_exp_unsafe() + e0 - mb.real_exp_unsafe();
        auto r2  = r1.real_exp(e3);
        return r2;
    }
}

#endif
