#ifndef xpack_math_div
#define xpack_math_div
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_div
#include"define/base_type.hpp"
#include"define/mfxx.hpp"
#include"instruction/mod.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_div{
    template<class type> 
    inline quo_rem_pair<type> div_unsafe_core(type a, type b){
        auto tmp = a / b;
        auto que = (type)(i64)tmp;
        auto rem = tmp - que;
        return quo_rem_pair<type>{ que, rem };
    }

    inline quo_rem_pair<f32> div_unsafe(f32 a, f32 b){
        return div_unsafe_core<f32>(a, b);
    }

    inline quo_rem_pair<f64> div_unsafe(f64 a, f64 b){
        return div_unsafe_core<f64>(a, b);
    }

    inline quo_rem_pair<f80> div_unsafe(f80 a, f80 b){
        return div_unsafe_core<f80>(a, b);
    }

    template<class type>
    inline quo_rem_pair<type> div_core(type a, type b){
        auto ma = inc::mfxx{a};
        auto mb = inc::mfxx{b};
        
        if (ma.real_exp() - mb.real_exp() <= ma.decimal_bits()){
            return div_unsafe(a, b);
        }
        return quo_rem_pair<type>{ a / b, (type)inc::mod(a, b) };
    }

    inline quo_rem_pair<f32> div(f32 a, f32 b){
        return div_core<f32>(a, b);
    }

    inline quo_rem_pair<f64> div(f64 a, f64 b){
        return div_core<f64>(a, b);
    }

    inline quo_rem_pair<f80> div(f80 a, f80 b){
        return div_core<f80>(a, b);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::math_div::div;
}

namespace xuser::adv{
    using ::mixc::math_div::div_unsafe;
}
