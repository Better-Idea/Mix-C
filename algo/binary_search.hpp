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
        template<class item_t>
        struct binary_search{
        private:
            template<class seq_t>
            static auto match_template(
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
        public:
            static auto match(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_template(seq, seq.length(), value, 0, compare).match;
            }

            static uxx match(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                uxx                            start, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_template(seq, seq.length(), value, start, compare).match;
            }

            static uxx match(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                uxx                            length, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_template(seq, length, value, start, compare).match;
            }

            static uxx match_up(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_up(seq, value, 0, seq.length(), compare);
            }

            static uxx match_up(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                uxx                            start, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_up(seq, value, start, seq.length(), compare);
            }

            static uxx match_up(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                uxx                            start, 
                uxx                            length, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                auto result = match_template(seq, length, value, start, compare);
                return result.match == not_exist ? result.less_then_target : result.match;
            }

            static uxx match_down(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_down(seq, value, 0, seq.length(), compare);
            }

            static uxx match_down(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                uxx                            start, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                return match_down(seq, value, start, seq.length(), compare);
            }

            static uxx match_down(
                inc::can_random_access<item_t> seq,
                item_t const &                 value, 
                uxx                            start, 
                uxx                            length, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>) {
                auto result = match_template(seq, seq.length(), value, start, compare);
                return result.match == not_exist ? result.grater_then_target : result.match;
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_binary_search::binary_search;
}
