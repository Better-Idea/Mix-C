#ifndef xpack_algo_test_shuffle
#define xpack_algo_test_shuffle
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_test_shuffle::inc
#include"algo/shuffle.hpp"
#include"docker/array.hpp"
#include"docker/bit_indicator.hpp"
#include"macro/xassert.hpp"
#include"math/random.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_test_shuffle{
    xtest(shuffle) {
        using namespace inc;

        static array<uxx, 32> a, b;

        // 保证元素只是改变顺序，不存在遗漏
        auto test = [&](){
            for(uxx i = 1; i < a.length(); i++){
                for(uxx j = 0; j < 1000; j++){
                    b = a;
                    inc::shuffle(b.subseq(co{0, i}));
                    inc::bit_indicator<32> bmp;

                    for(uxx k = 0; k < i; k++){
                        xassert_eq(false, bmp.get(b[k] ^ 0x100000000ull));
                        bmp.set(b[k] ^ 0x100000000ull);
                    }
                }
            }
        };

        a.foreach([](uxx i, uxx & v){
            v = i;
        });
        test();

        a.foreach([](uxx i, uxx & v){
            v = i | 0x100000000ull;
        });
        test();
    };
}

#endif
