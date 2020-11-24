/* 模块：distinct
 * 类型：函数单体
 * 功能：去除一个序列重复的元素
 * 用法：
 * TODO ===========================================================
 * 
 * 注意：
 * - 要求该序列的元素可以两两比较
 */

#ifndef xpack_algo_distinct
#define xpack_algo_distinct
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_distinct::inc
#include"docker/hashmap.hpp"
#include"interface/can_callback.hpp"
#include"interface/unified_seq.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_distinct::origin{
    template<class functor_t, class seq_t> 
    concept can_distinct_alloc = inc::can_callback<
        functor_t, inc::unified_seq<seq_t>(uxx/*length*/)
    >;

    /* 函数：去除重复元素并保证顺序不改变
     * 参数：
     * - seq 为待去重的序列，该类型需要满足 can_unified_seqlize 约束
     * - alloc 为分配回调函数，期望签名如下：
     *      unified_seq<seq_t> operator()(uxx length)
     *   其中 seq_t 和待去重序列的类型保持一致，length 为去重后序列的长度
     * 返回：
     * - 从 alloc 中分配的序列
     */ 
    template<
        inc::can_unified_seqlize        seq_t,
        class                           alloc_t
    >
    requires(
        can_distinct_alloc<alloc_t, seq_t>
    )
    inline auto distinct(seq_t const & seq, alloc_t const & alloc){
        using namespace inc;
        using item_t = inc::item_origin_of<seq_t>;

        if (seq.length() == 0){
            return alloc(length{0});
        }

        hashmap<item_t, uxx>    map{
            length{seq.length()}
        };

        for(uxx i = 0, ii = 0; i < seq.length(); i++){
            // 注意：
            // hashmap_set_result_t    r;
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

        map.foreach([&](item_t const & key, uxx const & val){
            buffer[val] = key;
        });
        return buffer;
    }

    using inc::unified_seq;
    using inc::can_unified_seqlize;
}

#endif

xexport_space(mixc::algo_distinct::origin)
