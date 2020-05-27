#ifndef xpack_test_algo_remove
#define xpack_test_algo_remove
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_algo_insert
        #include"algo/remove.hpp"
        #include"define/base_type.hpp"
        #include"define/nullref.hpp"
        #include"docker/array.hpp"
        #include"docker/shared_ptr.hpp"
        #include"macro/xassert.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_algo_insert{
        xtest(remove){
            auto a = inc::array_view{ 0u,1,2 };
            auto b = a;
            auto l = 0;

            l = inc::remove<u32>(b, 0);
            xassert_eq(a.length() - 1, l);
            xassert_eq(a[1], b[0]);
            xassert_eq(a[2], b[1]);

            b = a;
            l = inc::remove<u32>(b, 1);
            xassert_eq(a.length() - 1, l);
            xassert_eq(a[0], b[0]);
            xassert_eq(a[2], b[1]);

            b = a;
            l = inc::remove<u32>(b, 2);
            xassert_eq(a.length() - 1, l);
            xassert_eq(a[0], b[0]);
            xassert_eq(a[1], b[1]);

            b = a;
            l = inc::remove<u32>(b, -1);
            xassert_eq(a.length() - 1, l);
            xassert_eq(a[0], b[0]);
            xassert_eq(a[1], b[1]);

            b = a;
            l = inc::remove<u32>(b, cc{0,1});
            xassert_eq(a.length() - 2, l);
            xassert_eq(a[2], b[0]);

            b = a;
            l = inc::remove<u32>(b, co{0});
            xassert_eq(0, l);

            auto used = inc::used_bytes();
            {
                using item_t = inc::shared_ptr<u32>;
                auto && a = inc::array_view{ 
                    item_t(ini_now, 1u),
                    item_t(ini_now, 2u),
                    item_t(ini_now, 3u),
                };

                inc::remove<item_t>(a, 0);
                xassert_eq(2, a[0]);
                xassert_eq(3, a[1]);
                xassert_eq(nullptr, a[2]);
            }
            xassert_eq(used, inc::used_bytes());
        };
    }
#endif
