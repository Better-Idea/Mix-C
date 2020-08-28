/*
二分查找 API —— binary_search
*/

#ifndef xpack_algo_binary_search
#define xpack_algo_binary_search
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_binary_search
#include"define/base_type.hpp"
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_binary_search{
    using can_search = inc::can_callback<ixx(uxx)>;

    template<class compare_invoke>
    inline auto match_core(uxx length, compare_invoke const & compare) {
        struct result_t {
            uxx match;
            uxx less_then_target;
            uxx grater_then_target;

            result_t() {
                match               = not_exist;
                less_then_target    = not_exist;
                grater_then_target  = not_exist;
            }
        };

        result_t result;
        i64      left   = 0;
        i64      center = i64(length >> 1);
        i64      right  = i64(length - 1);
        i64      backup = 0;
        ixx      cmp    = 0;

        for (; left <= right; center = (left + right) >> 1) {
            if (backup = center, cmp = compare(uxx(center)); cmp > 0) {
                right = center - 1;
            }
            else if (cmp < 0) {
                left = center + 1;
            }
            else {
                result.match = uxx(center);
                return result;
            }
        }

        if (cmp > 0) {
            result.grater_then_target = uxx(backup);

            if (backup > 0) {
                result.less_then_target = uxx(backup - 1);
            }
        }
        else {
            result.less_then_target = uxx(backup);

            if (backup + 1 < length) {
                result.grater_then_target = uxx(backup + 1);
            }
        }
        return result;
    }

    template<inc::unified_seq_t seq_t>
    inline auto match_template(
        seq_t                                       const & seq,
        inc::item_origin_of<seq_t>                  const & value,
        inc::can_compare< inc::item_origin_of<seq_t> >      compare) {

        inc::unified_seq<seq_t> list(seq);
        return match_core(list.length(), [&](uxx current){
            return compare(list[current], value);
        });
    }

    struct binary_search{
        // 说明：在升序序列中寻找刚好匹配搜索值的索引，如果不匹配则返回 not_exist
        template<inc::unified_seq_t seq_t>
        static uxx match(
            seq_t                                      const & seq,
            inc::item_origin_of<seq_t>                 const & value,
            inc::can_compare< inc::item_origin_of<seq_t> >     compare = 
            inc::default_compare< inc::item_origin_of<seq_t> >) {

            return match_template(seq, value, compare).match;
        }

        // 说明：在升序序列中寻找不小于搜索值的索引，如果不匹配则返回 not_exist
        template<inc::unified_seq_t seq_t>
        static uxx greater_equals(
            seq_t                                      const & seq,
            inc::item_origin_of<seq_t>                 const & value,
            inc::can_compare< inc::item_origin_of<seq_t> >     compare = 
            inc::default_compare< inc::item_origin_of<seq_t> >) {

            auto result = match_template(seq, value, compare);
            return result.match == not_exist ? result.grater_then_target : result.match;
        }

        // 说明：在升序序列中寻找不大于搜索值的索引，如果不匹配则返回 not_exist
        template<inc::unified_seq_t seq_t>
        static uxx less_equals(
            seq_t                                      const & seq,
            inc::item_origin_of<seq_t>                 const & value,
            inc::can_compare< inc::item_origin_of<seq_t> >     compare = 
            inc::default_compare< inc::item_origin_of<seq_t> >) {

            auto result = match_template(seq, value, compare);
            return result.match == not_exist ? result.less_then_target : result.match;
        }

        // 说明：在升序序列中寻找刚好匹配搜索值的索引，如果不匹配则返回 not_exist
        template<inc::unified_seq_t seq_t>
        static uxx match(uxx length, can_search const & compare) {
            return match_core(length, compare).match;
        }

        // 说明：在升序序列中寻找不小于搜索值的索引，如果不匹配则返回 not_exist
        template<inc::unified_seq_t seq_t>
        static uxx greater_equals(uxx length, can_search const & compare) {
            auto result = match_core(length, compare);
            return result.match == not_exist ? result.grater_then_target : result.match;
        }

        // 说明：在升序序列中寻找不大于搜索值的索引，如果不匹配则返回 not_exist
        template<inc::unified_seq_t seq_t>
        static uxx less_equals(uxx length, can_search const & compare) {
            auto result = match_core(length, compare);
            return result.match == not_exist ? result.less_then_target : result.match;
        }
    };
}

#endif

namespace xuser::inc{
    using ::mixc::algo_binary_search::binary_search;
}
