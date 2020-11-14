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
        auto && a = inc::array<u32, 32>();
        auto && b = inc::array_view{ 0u,0,2,3,0,0 };
        auto    c = b;
        auto    r = c.range(cc{2,3});

        inc::insert(r, 0, 1u);
        xfail_if(0 != c[1]);
        xfail_if(1 != c[2]);
        xfail_if(2 != c[3]);
        xfail_if(3 != c[4]);
        xfail_if(0 != c[5]);

        c = b;
        r = c.range(cc{2,3});
        inc::insert(r, 0, {1u,4u});
        xfail_if(0, c[1]);
        xfail_if(1, c[2]);
        xfail_if(4, c[3]);
        xfail_if(2, c[4]);
        xfail_if(3, c[5]);

        c = b;
        r = c.range(cc{2,3});
        inc::insert(r, -1, {1u,4u});
        xfail_if(0, c[1]);
        xfail_if(2, c[2]);
        xfail_if(3, c[3]);
        xfail_if(1, c[4]);
        xfail_if(4, c[5]);

        c = b;
        r = c.range(cc{3,2});
        inc::insert(r, -1, {1u,4u});
        xfail_if(4, c[0]);
        xfail_if(1, c[1]);
        xfail_if(2, c[2]);
        xfail_if(3, c[3]);
        xfail_if(0, c[4]);
        xfail_if(0, c[5]);

        c = b;
        r = c.range(cc{3,2});
        inc::insert(r, -2, {1u,4u});
        xfail_if(2, c[0]);
        xfail_if(4, c[1]);
        xfail_if(1, c[2]);
        xfail_if(3, c[3]);
        xfail_if(0, c[4]);
        xfail_if(0, c[5]);

        c = b;
        r = c.range(cc{3,2});
        inc::insert(r, -3, {1u,4u});
        xfail_if(2, c[0]);
        xfail_if(3, c[1]);
        xfail_if(4, c[2]);
        xfail_if(1, c[3]);
        xfail_if(0, c[4]);
        xfail_if(0, c[5]);

        c = b;
        r = c.range(cc{3,2});
        inc::insert(r, 0, 4u);
        xfail_if(2, c[1]);
        xfail_if(3, c[2]);
        xfail_if(4, c[3]);
        xfail_if(0, c[4]);
        xfail_if(0, c[5]);
    };
}

#endif
