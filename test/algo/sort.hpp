#ifndef xpack_algo_test_sort
#define xpack_algo_test_sort
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_test_sort
#include"algo/sort.hpp"
#include"docker/array.hpp"
#include"docker/bit_indicator.hpp"
#include"macro/xassert.hpp"
#include"math/random.hpp"

#include<array>
#include<algorithm>
#pragma pop_macro("xuser")

namespace mixc::algo_test_sort{
    xtest(sort) {
        using namespace inc;

        static std::array<u32, 100> a;
        static inc::array<u32, 100> b;

        for(uxx i = 1; i < 100; i++){
            for(uxx j = 0; j < 1000; j++){
                for(uxx k = 0; k < i; k++){
                    a[k] = b[k] = random<u32>();
                }
                auto subseq = b.subseq(co{0, i});
                std::sort(a.begin(), a.begin() + i);
                inc::sort::heap(subseq);

                for(uxx k = 0; k < subseq.length(); k++){
                    xassert_eq(a[k], subseq[k]);
                }
            }
        }
    };
}

#endif
