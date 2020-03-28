#ifndef xpack_algo_binary_search
#define xpack_algo_binary_search
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_binary_search
        #include"interface/can_compare.hpp"
        #include"interface/can_random_access.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_binary_search{
        template<class item_t, class seq_t>
        inline auto binary_search_template(
            seq_t const &            sequence, 
            uxx                      length, 
            item_t const &           value, 
            uxx                      start, 
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

            if (start >= length) {
                return result_t();
            }

            result_t result;
            seq_t &  seq    = (seq_t &)sequence;
            uxx      left   = start;
            uxx      center = length >> 1;
            uxx      right  = length - 1;
            uxx      backup = 0;
            ixx      cmp    = 0;

            for (; left <= right; center = (left + right) >> 1) {
                if (backup = center, cmp = compare(seq[center], value); cmp > 0) {
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
        inline auto binary_search(
            inc::can_random_access<item_t &> seq,
            item_t const &                   value, 
            inc::can_compare<item_t>         compare = inc::default_compare<item_t>) {
            return binary_search_template(seq, seq.length(), value, 0, compare).match;
        }

        template<class item_t>
        inline auto binary_search(
            inc::can_random_access<item_t &> seq,
            item_t const &                   value, 
            uxx                              start, 
            inc::can_compare<item_t>         compare = inc::default_compare<item_t>) {
            return binary_search_template(seq, seq.length(), value, start, compare).match;
        }

        template<class item_t>
        inline auto binary_search_match_up(
            inc::can_random_access<item_t> seq,
            item_t const &                 value, 
            inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
            auto result = binary_search_template(seq, seq.length(), value, 0, compare);
            return result.match == not_exist ? result.less_then_target : result.match;
        }

        template<class item_t>
        inline auto binary_search_match_up(
            inc::can_random_access<item_t> seq,
            item_t const &                 value, 
            uxx                            start, 
            inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
            auto result = binary_search_template(seq, seq.length(), value, start, compare);
            return result.match == not_exist ? result.less_then_target : result.match;
        }

        template<class item_t>
        inline auto binary_search_match_down(
            inc::can_random_access<item_t> seq,
            item_t const &                 value, 
            inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
            auto result = binary_search_template(seq, seq.length(), value, 0, compare);
            return result.match == not_exist ? result.grater_then_target : result.match;
        }

        template<class item_t>
        inline auto binary_search_match_down(
            inc::can_random_access<item_t> seq,
            item_t const &                 value, 
            uxx                            start, 
            inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
            auto result = binary_search_template(seq, seq.length(), value, start, compare);
            return result.match == not_exist ? result.grater_then_target : result.match;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_binary_search::binary_search;
    using ::mixc::algo_binary_search::binary_search_match_down;
    using ::mixc::algo_binary_search::binary_search_match_up;
}
