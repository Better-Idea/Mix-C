#ifndef xpack_algo_distinct
#define xpack_algo_distinct
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_distinct
#include"docker/hashmap.hpp"
#include"interface/can_callback.hpp"
#include"interface/unified_seq.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_distinct{
    using namespace inc;

    template<inc::unified_seq_t seq_t> 
    using distinct_alloc_invoke = inc::can_callback<
        unified_seq<seq_t>(uxx/*length*/)
    >;

    template<inc::unified_seq_t seq_t>
    inline auto distinct(
        seq_t const &                   seq, 
        distinct_alloc_invoke<seq_t>    alloc){

        if (seq.length() == 0){
            return alloc(length{0});
        }

        using item_t = inc::item_origin_of<seq_t>;
        hashmap_set_result_t    r;
        hashmap<item_t, uxx>    map(
            length{seq.length()}
        );

        for(uxx i = 0, ii = 0; i < seq.length(); i++){
            // 注意：
            // auto key = xref range[i]; // 错误0：获取指针导致 key 在 map 中始终是唯一的，实际上需要获取值
            // map.set(key, ii, xref r); // 错误1：覆盖 key 对应的 ii，导致索引被覆盖
            // 
            // // 如果之前不存在
            // if (r != hashmap_set_result_t::override){
            //     ii += 1;
            // }

            // 如果之前不存在
            if (auto &  key = seq[i]; not map.is_contains(key)){
                map.set(key, ii);
                ii += 1;
            }
        }

        auto && buffer = alloc(
            length{map.length()}
        );

        map.foreach([&](uxx index, item_t const & key, uxx const & val){
            buffer[val] = key;
        });
        return buffer;
    }
}

namespace mixc::algo_distinct::origin{
    using mixc::algo_distinct::distinct_alloc_invoke;
    using mixc::algo_distinct::distinct;
    using inc::unified_seq;
    using inc::unified_seq_t;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::algo_distinct::origin;
}
