#ifndef xpack_test_algo_insert
#define xpack_test_algo_insert
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_algo_insert
        #include"algo/insert.hpp"
        #include"define/base_type.hpp"
        #include"docker/array.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_algo_insert{
        xtest(insert){
            auto && a = inc::array<u32, 32>();
            auto && b = inc::array_view{ 0u,0,2,3,0,0 };
            auto    c = b;
            auto    r = c.range(cc{2,3});
            auto    l = uxx(0);

            l = inc::insert<u32>(r, 0, 1u);
            xassert_eq(3, l);
            xassert_eq(0, c[1]);
            xassert_eq(1, c[2]);
            xassert_eq(2, c[3]);
            xassert_eq(3, c[4]);
            xassert_eq(0, c[5]);

            c = b;
            r = c.range(cc{2,3});
            l = inc::insert<u32>(r, 0, {1u,4u});
            xassert_eq(4, l);
            xassert_eq(0, c[1]);
            xassert_eq(1, c[2]);
            xassert_eq(4, c[3]);
            xassert_eq(2, c[4]);
            xassert_eq(3, c[5]);

            c = b;
            r = c.range(cc{2,3});
            l = inc::insert<u32>(r, -1, {1u,4u});
            xassert_eq(4, l);
            xassert_eq(0, c[1]);
            xassert_eq(2, c[2]);
            xassert_eq(3, c[3]);
            xassert_eq(1, c[4]);
            xassert_eq(4, c[5]);

            c = b;
            r = c.range(cc{3,2});
            l = inc::insert<u32>(r, -1, {1u,4u});
            xassert_eq(4, l);
            xassert_eq(4, c[0]);
            xassert_eq(1, c[1]);
            xassert_eq(2, c[2]);
            xassert_eq(3, c[3]);
            xassert_eq(0, c[4]);
            xassert_eq(0, c[5]);

            c = b;
            r = c.range(cc{3,2});
            l = inc::insert<u32>(r, -2, {1u,4u});
            xassert_eq(4, l);
            xassert_eq(2, c[0]);
            xassert_eq(4, c[1]);
            xassert_eq(1, c[2]);
            xassert_eq(3, c[3]);
            xassert_eq(0, c[4]);
            xassert_eq(0, c[5]);

            c = b;
            r = c.range(cc{3,2});
            l = inc::insert<u32>(r, -3, {1u,4u});
            xassert_eq(4, l);
            xassert_eq(2, c[0]);
            xassert_eq(3, c[1]);
            xassert_eq(4, c[2]);
            xassert_eq(1, c[3]);
            xassert_eq(0, c[4]);
            xassert_eq(0, c[5]);

            c = b;
            r = c.range(cc{3,2});
            l = inc::insert<u32>(r, 0, 4u);
            xassert_eq(3, l);
            xassert_eq(2, c[1]);
            xassert_eq(3, c[2]);
            xassert_eq(4, c[3]);
            xassert_eq(0, c[4]);
            xassert_eq(0, c[5]);
        };
    }
#endif
