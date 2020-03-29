#ifndef xpack_algo_sort
#define xpack_algo_sort
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_sort
        #include"algo/heap_root.hpp"
        #include"define/base_type.hpp"
        #include"interface/can_compare.hpp"
        #include"interface/can_random_access.hpp"
        #include"macro/xcmp.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_sort{
        template<class item_t>
        struct sort{
            static void des(
                inc::can_random_access<item_t> sequence,
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>){

                for(uxx i = 1, length = sequence.length(); i < length; i++) {
                    inc::heap_root<item_t>::push(sequence, i, sequence[i], compare);
                }

                for(uxx i = sequence.length(); --i > 0; ) {
                    sequence[i] = inc::heap_root<item_t>::pop(sequence, i + 1, sequence[i], compare);
                }
            }

            static void asc(
                inc::can_random_access<item_t> sequence,
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>){

                des(sequence, [&]xcmp(item_t){
                    return compare(right, left);
                });
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_sort::sort;
}
