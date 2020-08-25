#ifndef xpack_algo_test_distinct
#define xpack_algo_test_distinct
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_test_distinct
#include"algo/distinct.hpp"
#include"docker/array.hpp"
#include"docker/bit_indicator.hpp"
#include"macro/xassert.hpp"
#include"math/random.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_test_distinct{
    xtest(distinct) {
        using namespace inc;
        
        static array<u32, 64> r;

        for(uxx i = 1; i < r.length(); i++){
            for(uxx j = 0; j < r.length(); j++){
                auto && bmp = bit_indicator<1024>();
                auto    seq = r.subseq(co{0, j});

                for(uxx k = 0; k < seq.length(); k++){
                    seq[k] = random<u32>() % i;
                    bmp.set(seq[k]);
                }

                auto new_list = inc::distinct(seq, [](uxx len) -> inc::unified_seq<decltype(r)>{
                    static u32 buffer[1024];
                    return { buffer, len, };
                });

                for(uxx k = 0; k < new_list.length(); k++){
                    auto exist = bmp.get(new_list[k]);
                    xassert(exist == true, i, j, k, new_list[k]);
                    bmp.reset(new_list[k]);
                }
            }
        }

    };
}

#endif
