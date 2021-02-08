#ifndef xpack_test_algo_sort
#define xpack_test_algo_sort
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_algo_sort::inc
#include"algo/sort.hpp"
#include"docker/array.hpp"
#include"macro/xassert.hpp"
#include"macro/xexport.hpp"
#include"math/random.hpp"
#include"utils/bits_indicator.hpp"

#include<array>
#include<algorithm>
#pragma pop_macro("xuser")

namespace mixc::test_algo_sort::origin{
    xtest("sort") {
        using namespace inc;

        static std::array<u32, 100> a;
        static inc::array<u32, 100> b;

        for(uxx i = 1; i < 100; i++){
            for(uxx j = 0; j < 1000; j++){
                for(uxx k = 0; k < i; k++){
                    a[k] = b[k] = random<u32>();
                }
                auto subseq = b.range(co{0, i});
                std::sort(a.begin(), a.begin() + i);
                inc::sort::heap(subseq);

                for(uxx k = 0; k < subseq.length(); k++){
                    xfail_if(a[k] != subseq[k], k, a[k], subseq[k]);
                }
            }
        }
    };
}

#endif

xexport_space(mixc::test_algo_sort::origin)