#ifndef xpack_math_random
#define xpack_math_random
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_random::inc
#include"define/base_type.hpp"
#include"define/nan.hpp"
#include"instruction/ring_shift_left.hpp"
#include"instruction/time_stamp.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_random::inc{
    inline u64 x = inc::time_stamp();
    inline u64 y = inc::time_stamp() * magic_number;

    inline u64 random() {
        constexpr u64 change_period = 0x3f;

        // 由于线程会有不同的栈，所以这里可以补充随机性
        uxx dummy;
        uxx addition    = uxx(& dummy);

        // 这里维持了随机的不确定性
        if ((x & change_period) == 0){
            x          += inc::time_stamp();
        }

        // 这里维持了随机的稳定性
        x              += inc::ring_shift_left(addition, uxx(y));
        x              += inc::ring_shift_left(y, uxx(x));
        y              += inc::ring_shift_left(x, uxx(y));

        // 只要没人知道它下一步会变成什么，那么它就是随机的。
        return y;
    }
}

namespace mixc::math_random{
    template<class type_t>
    inline type_t random(){
        if constexpr (inc::is_same<type_t, f32>){
            union {
                u64 u;
                f32 candidate[2];
            } v;

            while(true){
                if (v.u = inc::random(); v.candidate[0] != inc::nan){
                    return v.candidate[0];
                }
                else if (v.candidate[1] != inc::nan){
                    return v.candidate[1];
                }
            }
        }
        else if constexpr (inc::is_same<type_t, f64>){
            union {
                u64 u;
                f64 candidate;
            } v;

            while(true){
                if (v.u = inc::random(); v.candidate != inc::nan){
                    return v.candidate;
                }
            }
        }
        else if constexpr (inc::is_same<type_t, bool>){
            return inc::random() % 2 == 0;
        }
        else{
            return type_t(inc::random());
        }
    }
}

#endif

xexport(mixc::math_random::random)