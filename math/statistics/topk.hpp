#ifndef xpack_math_statistics_topk
#define xpack_math_statistics_topk
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_statistics_topk::inc
#include"algo/heap_root.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/min.hpp"
#include"memop/cmp.hpp"
#include"memop/swap.hpp"
#include"meta/item_origin_of.hpp"
#include"interface/unified_seq.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_statistics_topk::origin{
    enum class topk_with{
        non_sort,
        des_sort,
        asc_sort,
    };

    template<inc::can_unified_seqlize seq_k_t, inc::can_unified_seqlize seq_src_t>
    inline seq_k_t & topk(seq_k_t const & k, seq_src_t const & source, topk_with mode = topk_with::des_sort){
        using item_t = inc::item_origin_of<seq_k_t>;

        auto    n       = (uxx)0;
        auto    len     = (uxx)inc::min(k.length(), source.length());
        auto &  kref    = (seq_k_t &)k;

        for(uxx i = 0; i < len; i++){
            inc::heap_root::push(kref, i, source[i]); // 构建小根堆
        }
        for(uxx i = len; i < source.length(); i++){
            if (auto cur = source[i]; cur <= kref[0]){ // kref[0] 是 topk 数组中最小的元素
                continue;
            }
            else{
                inc::heap_root::pop(kref, kref.length(), kref[len - 1]);
                inc::heap_root::push(kref, kref.length(), cur);
            }
        }

        if (mode == topk_with::non_sort){
            return kref;
        }
        for(uxx i = len; --i > 0; ) {
            kref[i] = inc::heap_root::pop(kref, i + 1, kref[i]);
        }
        if (mode == topk_with::asc_sort) for (uxx i = 0; i < len / 2; i++){
            inc::swap(xref kref[i], xref kref[len - i - 1]);
        }
        return kref;
    }
}

#endif

xexport_space(mixc::math_statistics_topk::origin)