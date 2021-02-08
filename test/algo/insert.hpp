#ifndef xpack_test_algo_insert
#define xpack_test_algo_insert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_algo_insert::inc
#include"algo/insert.hpp"
#include"docker/array.hpp"
#include"macro/xassert.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_algo_insert{
    xtest("insert"){
        auto && a       = inc::array<u32, 32>();
        auto && b       = inc::array_view{ 0u,0,2,3,0,0 };
        auto    c       = b;

        // 指向 c[2, 3] 区间的 2 个元素
        auto    r       = c.subseq(cc{2,3}); 
        auto    length  = uxx(0);

        // 插入一个元素后变成 3 个
        length          = inc::insert(r, 0, 1u);
        xfail_ifne(length, 3);
        xfail_ifne(1, r[0]);
        xfail_ifne(2, r[1]);
        xfail_ifne(3, r[2]);

        c               = b;
        r               = c.subseq(cc{2,3});
        inc::insert(r, 0, {1u, 4u});
        xfail_ifne(0, c[1]);
        xfail_ifne(1, c[2]);
        xfail_ifne(4, c[3]);
        xfail_ifne(2, c[4]);
        xfail_ifne(3, c[5]);

        c = b;
        r = c.subseq(cc{2,3});
        inc::insert(r, -1, {1u,4u});
        xfail_ifne(0, c[1]);
        xfail_ifne(2, c[2]);
        xfail_ifne(3, c[3]);
        xfail_ifne(1, c[4]);
        xfail_ifne(4, c[5]);

        c = b;
        r = c.subseq(cc{3,2});
        inc::insert(r, -1, {1u,4u});
        xfail_ifne(4, c[0]);
        xfail_ifne(1, c[1]);
        xfail_ifne(2, c[2]);
        xfail_ifne(3, c[3]);
        xfail_ifne(0, c[4]);
        xfail_ifne(0, c[5]);

        c = b;
        r = c.subseq(cc{3,2});
        inc::insert(r, -2, {1u,4u});
        xfail_ifne(2, c[0]);
        xfail_ifne(4, c[1]);
        xfail_ifne(1, c[2]);
        xfail_ifne(3, c[3]);
        xfail_ifne(0, c[4]);
        xfail_ifne(0, c[5]);

        c = b;
        r = c.subseq(cc{3,2});
        inc::insert(r, -3, {1u,4u});
        xfail_ifne(2, c[0]);
        xfail_ifne(3, c[1]);
        xfail_ifne(4, c[2]);
        xfail_ifne(1, c[3]);
        xfail_ifne(0, c[4]);
        xfail_ifne(0, c[5]);

        c = b;
        r = c.subseq(cc{3,2});
        inc::insert(r, 0, 4u);
        xfail_ifne(2, c[1]);
        xfail_ifne(3, c[2]);
        xfail_ifne(4, c[3]);
        xfail_ifne(0, c[4]);
        xfail_ifne(0, c[5]);
    };
}

#endif
