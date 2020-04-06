/*
二分查找 API —— binary_search
*/

#ifndef xpack_algo_binary_search
#define xpack_algo_binary_search
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_binary_search
        #include"define/base_type.hpp"
        #include"interface/can_compare.hpp"
        #include"macro/xrange.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_binary_search{
        template<class item_t>
        inline auto match_template(
            inc::ranger<item_t> &    range,
            item_t const &           value,
            inc::can_compare<item_t> compare) {

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
            uxx      length = range.length();
            uxx      left   = 0;
            uxx      center = length >> 1;
            uxx      right  = length - 1;
            uxx      backup = 0;
            ixx      cmp    = 0;

            for (; left <= right; center = (left + right) >> 1) {
                if (backup = center, cmp = compare(range[center], value); cmp > 0) {
                    if (right = center - 1; i64(ixx(right)) < 0){
                        break;
                    }
                }
                else if (cmp < 0) {
                    left = center + 1;
                }
                else {
                    result.match = center;
                    return result;
                }
            }

            if (cmp > 0) {
                result.grater_then_target = backup;

                if (backup > 0) {
                    result.less_then_target = backup - 1;
                }
            }
            else {
                result.less_then_target = backup;

                if (backup + 1 < length) {
                    result.grater_then_target = backup + 1;
                }
            }
            return result;
        }
        
        template<class item_t>
        struct binary_search{
            // 说明：在升序序列中寻找刚好匹配搜索值的索引，如果不匹配则返回 not_exist
            static auto match(
                inc::ranger<item_t>      range,
                item_t const &           value, 
                inc::can_compare<item_t> compare = inc::default_compare<item_t>) {

                return match_template(range, value, compare).match;
            }

            // 说明：在升序序列中寻找不小于搜索值的索引，如果不匹配则返回 not_exist
            static uxx match_up(
                inc::ranger<item_t>      range,
                item_t const &           value, 
                inc::can_compare<item_t> compare = inc::default_compare<item_t>) {

                auto result = match_template(range, value, compare);
                return result.match == not_exist ? result.less_then_target : result.match;
            }

            // 说明：在升序序列中寻找不大于搜索值的索引，如果不匹配则返回 not_exist
            static uxx match_down(
                inc::ranger<item_t>      range,
                item_t const &           value,
                inc::can_compare<item_t> compare = inc::default_compare<item_t>) {

                auto result = match_template(range, value, compare);
                return result.match == not_exist ? result.grater_then_target : result.match;
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_binary_search::binary_search;
}
