#ifndef xpack_algo_sort
#define xpack_algo_sort
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_sort::inc
#include"algo/heap_root.hpp"
#include"docker/bit_indicator.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"meta/is_float.hpp"
#include"math/const.hpp"
#include"macro/xcmp.hpp"
#include"memop/swap.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_sort{
    constexpr u08 type_unsigned     = 0x00;
    constexpr u08 type_signed       = 0x80;
    constexpr u08 type_float_pos    = type_signed;
    constexpr u08 type_float_neg    = type_unsigned;
    constexpr u08 mode_asc      = 0x00;
    constexpr u08 mode_des      = 0xff;

    template<class counter_t, uxx i, u08 type, u08 mode, class seq_t>
    inline void radix_sort_core(seq_t & r, uxx offset, uxx length){
        using item_t        = inc::item_origin_of<seq_t>;

        enum{ top_i = sizeof(item_t) - 1 };
        counter_t sum0[256];
        counter_t sum1[256];
        inc::bit_indicator<256> idc;

        auto map = [&](uxx index){
            u08 hex = (u08p(xref r[index + offset])[i]);

            if constexpr (not inc::is_float<item_t>){ // 整数
                u08 idx = hex ^ type ^ mode;
                return idx;
            }
            else if constexpr (i == top_i){ // 浮点的最高字节
                u08 idx = hex & 0x80 ? ~hex : hex | 0x80;
                return idx;
            }
            else { // 浮点剩余字节按无符号数排序
                u08 idx = hex ^ type_unsigned ^ mode;
                return idx;
            }
        };

        for(uxx j = 0; j < length; j++){
            u08 idx         = map(j);
            // xhint(i, idx);

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
            idx         = map(j);
            ofs         = sum1[idx];
            // xhint(idx, ofs)

            if (j != ofs){
                inc::swap(xref r[j + offset], xref r[ofs + offset]);
            }
            else{
                j      += 1;
            }

            if (sum1[idx] += 1; sum1[idx] == sum0[idx]){
                idc.reset(idx);
                idx     = idc.index_of_first_set();
                if (idx == not_exist){
                    break;
                }
                j       = sum1[idx];
            }
        }

        if constexpr (i > 0){
            // 对子分区排序，按下一个字节排
            // 此时 sum0 = sum1
            for(t = 0; not_exist != (j = idct.pop_first()); t = sum0[j]){
                auto new_length = sum0[j] - t;
                // xhint(sum0[j], t);

                if (new_length == 1){ // 只有一个元素不用排序
                    continue;
                }
                if (new_length == 2){ // 两个元素
                    constexpr auto xor_condition = inc::is_float<item_t> and type == type_float_neg ?
                        mode == (mode_des ^ 0xff) : mode == mode_des;

                    if ((r[t + offset] > r[t + offset + 1]) ^ (xor_condition)){
                        inc::swap(xref r[t + offset], xref r[t + offset + 1]);
                    }
                }
                // TODO：else if (new_length < small_length) ========================================================================
                else if (inc::is_float<item_t> and i == top_i and j < 0x80){ // 浮点负数部分，低位字节按相反的模式排序
                    radix_sort_core<counter_t, i - 1, type_float_neg, mode ^ 0xff, seq_t>(r, t + offset, new_length);
                }
                else{
                    constexpr auto convert_type = inc::is_float<item_t> ? // 如果是浮点元素，则保持 type 不变
                        type : type_unsigned;
                    radix_sort_core<counter_t, i - 1,  convert_type, mode, seq_t>(r, t + offset, new_length);
                }
            }
        }
    }

    struct sort{
        template<
            inc::can_unified_seqlize    seq_t,
            class                       cmp_t,
            class                       item_t = inc::item_origin_of<seq_t>
        >
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        static void heap(
            seq_t   const & seq,
            cmp_t   const & compare = inc::default_compare<item_t>){

            inc::unified_seq<seq_t> list{seq};

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

        template<inc::can_unified_seqlize seq_t>
        static void radix_asc(seq_t const & seq){
            using item_t = inc::item_origin_of<seq_t>;
            xgen(u08, mode_asc)
            xgen(u16, mode_asc)
            xgen(u32, mode_asc)
            xgen(u64, mode_asc);
        }

        template<inc::can_unified_seqlize seq_t>
        static void radix_des(seq_t const & seq){
            using item_t = inc::item_origin_of<seq_t>;

            xgen(u08, mode_des)
            xgen(u16, mode_des)
            xgen(u32, mode_des)
            xgen(u64, mode_des);
        }

        #undef  xgen
    };
}

#endif

xexport(mixc::algo_sort::sort)
