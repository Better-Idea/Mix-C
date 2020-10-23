#ifndef xpack_algo_sort
#define xpack_algo_sort
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_sort
#include"algo/heap_root.hpp"
#include"docker/bit_indicator.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xcmp.hpp"
#include"memop/swap.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_sort{
    template<uxx i, class seq_t>
    inline void radix_sort_core(seq_t r){
        uxx sum0[256] = {0};
        uxx sum1[256] = {0};
        inc::bit_indicator<256> idc;

        for(uxx j = 0; j < r.length(); j++){
            u08 hex     = u08p(xref r[j])[i];
            sum0[hex]  += 1;
            idc.set(hex);
        }

        auto idct       = idc;
        auto j          = idct.pop_first();
        auto t          = sum0[j];

        // origin: 1 0 2 0 0 5 1 0
        // sum0  : 1 0 3 0 0 8 9 0
        // sum1  : 0 0 1 0 0 3 8 0 
        while(not_exist != (j = idct.pop_first())){
            sum0[j]    += t;
            sum1[j]     = t;
            t           = sum0[j];
        }

        idct = idc;

        for(uxx j = 0, hex, ofs; hex != not_exist; ){
            hex         = u08p(xref r[j])[i];
            ofs         = sum1[hex];

            if (j != ofs){
                inc::swap(xref r[j], xref r[ofs]);
            }
            else{
                j      += 1;
            }

            if (sum1[hex] += 1; sum1[hex] == sum0[hex]){
                idc.reset(hex);
                hex     = idc.index_of_first_set();
                j       = sum1[hex];
            }
        }

        if constexpr (i > 0){
            // 对子分区排序，按下一个字节排
            // 此时 sum0 = sum1
            for(t = 0; not_exist != (j = idct.pop_first()); t = sum0[j]){
                if (auto new_length = sum0[j] - t - 1; new_length > 1){
                    auto new_seq    = r.subseq(co{t, sum0[j]});
                    radix_sort_core<i - 1, seq_t>(new_seq);
                }
            }
        }
    }

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

        // 注意：
        // 实验性功能
        // 目前只支持无符号整数的快速排序
        template<inc::unified_seq_t seq_t>
        static void radix(seq_t const & seq){
            using item_t = inc::item_origin_of<seq_t>;
            radix_sort_core<sizeof(item_t) - 1>(
                inc::unified_seq<seq_t>(seq)
            );
        }
    };
}

#endif

xexport(mixc::algo_sort::sort)
