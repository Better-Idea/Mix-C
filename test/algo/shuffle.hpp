#ifndef xpack_test_algo_shuffle
#define xpack_test_algo_shuffle
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_algo_shuffle::inc
#include"algo/shuffle.hpp"
#include"docker/array.hpp"
#include"macro/xassert.hpp"
#include"math/random.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_algo_shuffle{
    xtest("shuffle") {
        using namespace inc;

        static array<uxx, 32> a, b;

        // 保证元素只是改变顺序，不存在遗漏
        auto test = [&](){
            for(uxx i = 1; i < a.length(); i++){
                for(uxx j = 0; j < 1000; j++){
                    b = a;
                    inc::shuffle(b.subseq(co{0, i}));
                    inc::bits_indicator<32> bmp;

                    for(uxx k = 0; k < i; k++){
                        xfail_if(b[k] >= i);
                        xfail_if(bmp.get(b[k]));
                        bmp.set(b[k]);
                    }
                }
            }
        };

        a.foreach([](uxx i, uxx & v){
            v = i;
        });

        test();
    };
}

#endif
