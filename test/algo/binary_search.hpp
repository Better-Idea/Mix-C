#ifndef xpack_algo_test_binary_search
#define xpack_algo_test_binary_search
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_algo_binary_search::inc
#include"algo/binary_search.hpp"
#include"docker/array.hpp"
#include"macro/xassert.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_algo_binary_search{
    xtest("binary_search") {
        using namespace inc;
        array<uxx, 128> c;

        auto search = [&](uxx length, auto target, auto mode){
            if (length == 0){
                return not_exist;
            }
            for (uxx i = 0; i < length; i++){
                if (c[i] == target){
                    return i;
                }
                if (c[i] > target){
                    switch(mode){
                    case 0: break;
                    case 1: return i - 1;
                    case 2: return i;
                    }
                }
            }
            if (mode == 1 && c[length - 1] < target){
                return length - 1;
            }
            return not_exist;
        };

        // 生成用例
        for (uxx i = 0; i < c.length(); i++){
            c[i] = i * 2 + 1;
        }

        uxx actual;
        uxx wanted;

        // 对比测试
        for (uxx length = 1; length <= c.length(); length++){
            auto r = c.subseq(co{0, length});

            for (uxx find = 0; find < length * 2 + 2; find++){
                actual = inc::binary_search::match(r, find);
                wanted = search(length, find, 0);
                xfail_if(actual != wanted and c[actual] != c[wanted], length, find, actual, wanted);

                actual = inc::binary_search::less_equals(r, find);
                wanted = search(length, find, 1);
                xfail_if(actual != wanted and c[actual] != c[wanted], length, find, actual, wanted);

                actual = inc::binary_search::greater_equals(r, find);
                wanted = search(length, find, 2);
                xfail_if(actual != wanted and c[actual] != c[wanted], length, find, actual, wanted);
            }
        }
    };
}

#endif
