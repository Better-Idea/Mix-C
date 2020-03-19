#pragma once
#include"define/base_type.hpp"
#include"define/nan.hpp"
#include"instruction/ring_shift_left.hpp"
#include"instruction/time_stamp.hpp"
#include"meta/is_same.hpp"

namespace mixc{
    namespace inner_random{
        inline u64 x = mixc::time_stamp();
        inline u64 y = mixc::time_stamp() * magic_number;
        
        inline u64 random() {
            constexpr u64 change_period = 0x3f;

            if ((y & change_period) == 0){
                y += mixc::time_stamp();
            }

            x += mixc::ring_shift_left(x, y);
            y += mixc::ring_shift_left(y, x);
            return y;
        }
    }


    template<class type>
    inline type random(){
        if constexpr (is_same<type, f32>){
            union {
                u64 u;
                f32 candidate[2];
            } v ;

            while(true){
                if (v.u = inner_random::random(); v.candidate[0] != nan){
                    return v.candidate[0];
                }
                else if (v.candidate[1] != nan){
                    return v.candidate[1];
                }
            }
        }
        else if constexpr (is_same<type, f64>){
            union {
                u64 u;
                f64 candidate;
            } v ;

            while(true){
                if (v.u = inner_random::random(); v.candidate != nan){
                    return v.candidate;
                }
            }
        }
        else{
            return type(inner_random::random());
        }
    }
}
