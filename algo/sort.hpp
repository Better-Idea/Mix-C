#ifndef xpack_algo_sort
#define xpack_algo_sort
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_sort
        #include"algo/heap_root.hpp"
        #include"define/base_type.hpp"
        #include"interface/can_compare.hpp"
        #include"macro/xcmp.hpp"
        #include"macro/xrange.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_sort{
        template<class item_t>
        struct sort{
            static void des(
                inc::ranger<item_t>      range,
                inc::can_compare<item_t> compare = inc::default_compare<item_t>){

                for(uxx i = 1, length = range.length(); i < length; i++) {
                    inc::heap_root<item_t>::push(range, i, range[i], compare);
                }

                for(uxx i = range.length(); --i > 0; ) {
                    range[i] = inc::heap_root<item_t>::pop(range, i + 1, range[i], compare);
                }
            }

            static void asc(
                inc::ranger<item_t>      range,
                inc::can_compare<item_t> compare = inc::default_compare<item_t>){

                des(range, [&]xcmp(item_t){
                    return compare(right, left);
                });
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_sort::sort;
}
