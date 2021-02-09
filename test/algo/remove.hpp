#ifndef xpack_test_algo_remove
#define xpack_test_algo_remove
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_algo_remove::inc
#include"algo/remove.hpp"
#include"define/nullref.hpp"
#include"docker/array.hpp"
#include"docker/shared.hpp"
#include"macro/xassert.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_algo_remove{
    xtest("remove"){
        auto a = inc::array_view{ 0u, 1u, 2u };
        auto b = a;
        auto l = 0;

        l = inc::remove(b, 0);
        xfail_ifne(a.length() - 1, l);
        xfail_ifne(a[1], b[0]);
        xfail_ifne(a[2], b[1]);

        b = a;
        l = inc::remove(b, 1);
        xfail_ifne(a.length() - 1, l);
        xfail_ifne(a[0], b[0]);
        xfail_ifne(a[2], b[1]);

        b = a;
        l = inc::remove(b, 2);
        xfail_ifne(a.length() - 1, l);
        xfail_ifne(a[0], b[0]);
        xfail_ifne(a[1], b[1]);

        b = a;
        l = inc::remove(b, -1);
        xfail_ifne(a.length() - 1, l);
        xfail_ifne(a[0], b[0]);
        xfail_ifne(a[1], b[1]);

        b = a;
        l = inc::remove(b, cc{0,1});
        xfail_ifne(a.length() - 2, l);
        xfail_ifne(a[2], b[0]);

        b = a;
        l = inc::remove(b, co{0});
        xfail_ifne(0, l);

        auto used = inc::used_bytes();
        {
            using item_t = inc::shared<u32>;

            inc::array<item_t, 3> a { 
                inc::init_by{ 1u },
                inc::init_by{ 2u },
                inc::init_by{ 3u }
            };

            inc::remove(a, 0);
            xfail_ifne(2,               *a[0]);
            xfail_ifne(3,               *a[1]);
            xfail_if  (nullptr != a[2]);
        }

        xfail_ifne(used, inc::used_bytes());
    };
}

#endif
