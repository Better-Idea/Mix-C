#ifndef xpack_algo_sort
#define xpack_algo_sort
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_sort
#include"algo/heap_root.hpp"
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xcmp.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_sort{
    struct sort{
        template<inc::unified_seq_t seq_t>
        static void heap(
            seq_t                      const & seq,
            inc::can_compare<
                inc::item_origin_of<seq_t>
            >                                   compare = 
            inc::default_compare<
                inc::item_origin_of<seq_t>
            >){
            
            inc::unified_seq<seq_t> list(seq);

            using item_t = inc::item_origin_of<seq_t>;
            auto neg = [&]xcmp(item_t){
                return compare(right, left);
            };

            for(uxx i = 1; i < list.length(); i++) {
                inc::heap_root::push(list, i, list[i], neg);
            }

            for(uxx i = list.length(); --i > 0; ) {
                list[i] = inc::heap_root::pop(list, i + 1, list[i], neg);
            }
        }
    };
}

#endif

namespace xuser::inc{
    using ::mixc::algo_sort::sort;
}
