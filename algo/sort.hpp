#ifndef xpack_algo_sort
#define xpack_algo_sort
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_sort::inc
#include"algo/heap_root.hpp"
#include"docker/bit_indicator.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"math/const.hpp"
#include"macro/xcmp.hpp"
#include"memop/swap.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_sort{
    constexpr u08 type_unsigned = 0x00;
    constexpr u08 type_signed   = 0x80;
    constexpr u08 mode_asc      = 0x00;
    constexpr u08 mode_des      = 0xff;

    template<class counter_t, uxx i, u08 type, u08 mode, class seq_t>
    inline void radix_sort_core(seq_t & r, uxx offset, uxx length){
        using item_t        = inc::item_origin_of<seq_t>;

        constexpr
        uxx       top_i     = sizeof(item_t) - 1;
        counter_t sum0[256];
        counter_t sum1[256];
        inc::bit_indicator<256> idc;

        for(uxx j = 0; j < length; j++){
            u08 hex         = (u08p(xref r[j + offset])[i]);
            u08 idx         = (hex + type) ^ mode;
            // xhint(hex, idx)

            if (idc.get(idx) == false){
                idc.set(idx);
                sum0[idx]   = 1;
            }
            else{
                sum0[idx]  += (1);
            }
        }

        auto idct           = idc;
        auto j              = idct.pop_first();
        auto t              = sum0[j];
        sum1[j]             = 0;

        // origin: 1 0 2 0 0 5 1 0
        // sum0  : 1 0 3 0 0 8 9 0
        // sum1  : 0 0 1 0 0 3 8 0 
        while(not_exist != (j = idct.pop_first())){
            sum0[j]    += t;
            sum1[j]     = t;
            t           = sum0[j];
        }

        idct = idc;

        for(uxx j = 0, idx, ofs;;){
            u08 hex     = u08p(xref r[j + offset])[i];
            idx         = u08((hex + type) ^ mode);
            ofs         = sum1[idx];
            // xhint(hex, idx, ofs)

            if (j != ofs){
                inc::swap(xref r[j + offset], xref r[ofs + offset]);
            }
            else{
                j      += 1;
            }

            if (sum1[idx] += 1; sum1[idx] == sum0[idx]){
                idc.reset(idx);
                idx     = idc.index_of_first_set();
                j       = sum1[idx];

                if (idx == not_exist){
                    break;
                }
            }
        }

        if constexpr (i > 0){
            // 对子分区排序，按下一个字节排
            // 此时 sum0 = sum1
            for(t = 0; not_exist != (j = idct.pop_first()); t = sum0[j]){
                auto new_length = sum0[j] - t;
                 //xhint(sum0[j], t);

                if (new_length == 1){ // 只有一个元素不用排序
                    continue;
                }
                if (new_length == 2){ // 两个元素
                    if ((r[t + offset] > r[t + offset + 1]) ^ (mode == mode_des)){
                        inc::swap(xref r[t + offset], xref r[t + offset + 1]);
                    }
                }
                // TODO：else if (new_length < small_length) ========================================================================
                else{
                    radix_sort_core<counter_t, i - 1, type_unsigned, mode, seq_t>(r, t + offset, new_length);
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

        #define xgen(type,mode)                                 \
        if (seq.length() <= inc::max_value_of<type>){           \
            radix_sort_core<type,                               \
                (sizeof(item_t) - 1),                           \
                (item_t(-1) > 0) ? type_unsigned : type_signed, \
                (mode_asc)                                      \
            >(                                                  \
                (seq_t &)(seq), 0, seq.length()                 \
            );                                                  \
        } else

        template<inc::unified_seq_t seq_t>
        static void radix_asc(seq_t const & seq){
            using item_t = inc::item_origin_of<seq_t>;
            xgen(u08, mode_asc)
            xgen(u16, mode_asc)
            xgen(u32, mode_asc)
            xgen(u64, mode_asc);
        }

        template<inc::unified_seq_t seq_t>
        static void radix_des(seq_t const & seq){
            using item_t = inc::item_origin_of<seq_t>;

            xgen(u08, mode_des)
            xgen(u16, mode_des)
            xgen(u32, mode_des)
            xgen(u64, mode_des);
        }
    };
}

#endif

xexport(mixc::algo_sort::sort)
