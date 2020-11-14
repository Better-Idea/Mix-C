#ifndef xpack_test_auto_test
#define xpack_test_auto_test
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_auto_test::inc
#include"define/base_type.hpp"
#include"dumb/place_holder.hpp"
#include"macro/xassert.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_auto_test::origin{
    template<uxx i>
    inline auto test(inc::place_holder<i>){
        return nullptr;
    }

    inline inc::place_holder<not_exist> test(inc::place_holder<__COUNTER__>){
        return inc::place_holder<not_exist>{};
    }

    template<uxx i = 0>
    inline void auto_test(){
        using namespace xuser;
        using namespace inc;

        using t = decltype(test(place_holder<i>{}));

        if constexpr (is_same<t, place_holder<not_exist>>){
            return;
        }
        else {
            if constexpr (not is_same<t, decltype(nullptr)>){
                test(place_holder<i>{});
            }
            auto_test<i + 1>();
        }
    }
}

#endif

xexport_space(mixc::test_auto_test::origin);