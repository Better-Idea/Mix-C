#ifndef xpack_algo_distinct
#define xpack_algo_distinct
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_distinct
#include"docker/hashmap.hpp"
#include"interface/ranger.hpp"
#include"interface/can_callback.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_distinct{
    using namespace inc;

    template<class item_t>
    using distinct_alloc_invoke = can_callback<ranger<item_t>(length)>;

    template<class item_t>
    inline ranger<item_t> distinct(ranger<item_t> range, distinct_alloc_invoke<item_t> alloc){
        if (range.length() == 0){
            return alloc(length{0});
        }

        hashmap<item_t *, uxx> map(
            length{range.length()}
        );

        for(uxx i = 0, ii = 0; i < range.length(); i++){
            if (auto key = xref range[i]; not map.is_contains(key)){
                map.set(key, ii);
                ii += 1;
            }
        }

        auto && buffer = alloc(
            length{map.length()}
        );

        map.foreach([&](uxx index, item_t * const & key, uxx const & val){
            buffer[val] = *key;
        });

        return buffer;
    }
}

namespace mixc::algo_distinct::origin{
    using mixc::algo_distinct::distinct_alloc_invoke;
    using mixc::algo_distinct::distinct;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::algo_distinct::origin;
}