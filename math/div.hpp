#ifndef xpack_math_div
#define xpack_math_div
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_div
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_div{
    template<class type>
    struct quo_rem_pair{
        type quotient;
        type remainder;
    };

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
}

#endif

namespace xuser::adv{
    using ::mixc::math_div::div_unsafe;
}
