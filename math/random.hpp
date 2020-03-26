#ifndef xpack_math_random
#define xpack_math_random
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_random
        #include"define/base_type.hpp"
        #include"define/nan.hpp"
        #include"instruction/ring_shift_left.hpp"
        #include"instruction/time_stamp.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_random{
        namespace inner{
            inline u64 x;
            inline u64 y;

            struct __ini__{
                __ini__(){
                    x = inc::time_stamp();
                    y = inc::time_stamp() * magic_number;
                }
            } __ini__;

            inline u64 random() {
                constexpr u64 change_period = 0x3f;

                if ((y & change_period) == 0){
                    y += inc::time_stamp();
                }

                x += inc::ring_shift_left(x, y);
                y += inc::ring_shift_left(y, x);
                return y;
            }
        }


        template<class type>
        inline type random(){
            if constexpr (inc::is_same<type, f32>){
                union {
                    u64 u;
                    f32 candidate[2];
                } v ;

                while(true){
                    if (v.u = inner::random(); v.candidate[0] != inc::nan){
                        return v.candidate[0];
                    }
                    else if (v.candidate[1] != inc::nan){
                        return v.candidate[1];
                    }
                }
            }
            else if constexpr (inc::is_same<type, f64>){
                union {
                    u64 u;
                    f64 candidate;
                } v ;

                while(true){
                    if (v.u = inner::random(); v.candidate != inc::nan){
                        return v.candidate;
                    }
                }
            }
            else{
                return type(inner::random());
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::math_random::random;
}
