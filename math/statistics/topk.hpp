#ifndef xpack_math_statistics_topk
#define xpack_math_statistics_topk
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_statistics_topk::inc
#include"algo/heap_root.hpp"
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xexport.hpp"
#include"math/min.hpp"
#include"memop/cmp.hpp"
#include"memop/swap.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_statistics_topk::origin{
    enum class topk_with{
        non_sort,
        des_sort,
        asc_sort,
    };

    #define xheader                                                                 \
    template<                                                                       \
        inc::can_unified_seqlize seq_k_t,                                           \
        inc::can_unified_seqlize seq_src_t,                                         \
        class                    item_t  = inc::item_origin_of<seq_src_t>,          \
        class                    cmp_t   = decltype(inc::default_compare<item_t>)   \
    >

    xheader inline seq_k_t & topk(
        seq_k_t     const & k, 
        seq_src_t   const & source, 
        cmp_t       const & compare     = inc::default_compare<item_t>,
        topk_with           mode        = topk_with::des_sort){

        auto    n       = (uxx)0;
        auto    len     = (uxx)inc::min(k.length(), source.length());
        auto &  kref    = (seq_k_t &)k;

        for(uxx i = 0; i < len; i++){
            inc::heap_root::push(kref, i, source[i], compare); // 构建小根堆
        }
        for(uxx i = len; i < source.length(); i++){
            if (auto cur = source[i]; cur <= kref[0]){ // kref[0] 是 topk 数组中最小的元素
                continue;
            }
            else{
                inc::heap_root::pop(kref, len, cur/*insert*/, compare);
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

    xheader inline seq_k_t & topk(
        seq_k_t     const & k, 
        seq_src_t   const & source, 
        topk_with           mode){

        return topk(k, source, inc::default_compare<item_t>, mode);
    }

    xheader inline seq_k_t & topk(
        seq_k_t     const & k, 
        seq_src_t   const & source, 
        cmp_t       const & compare){

        return topk(k, source, compare);
    }

    #undef  xheader
}

#endif

xexport_space(mixc::math_statistics_topk::origin)