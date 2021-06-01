#ifndef xpack_algo_remove
#define xpack_algo_remove
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_remove::inc
#include"interface/seqptr.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xexport.hpp"
#include"macro/xindex_rollback.hpp"
#include"memop/copy.hpp"
#include"memop/swap.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_remove{
    #define xgen(tar)                       \
        range.normalize(tar.length());      \
        auto l = range.left();              \
        auto r = range.right();             \
                                            \
        if (l > r){                         \
            inc::swap(xref l, xref r);      \
        }

    template<class seq_tar_t, can_interval interval_t>
    inline uxx remove_core(seq_tar_t target, interval_t const & range){
        using item_t = inc::item_origin_of<seq_tar_t>;
        xgen(target);

        auto i   = 0;
        auto j   = target.length() - r - 1;
        auto len = j + l;

        while(i < j){
            target[l + i] = target[r + i + 1];
            i            += 1;
        }
        while(i < target.length()){
            target[l + i].~item_t();
            i += 1;
        }
        return len;
    }

    template<class seq_tar_t, class seq_src_t, can_interval interval_t>
    inline uxx remove_core(seq_tar_t target, seq_src_t source, interval_t const & range){
        xgen(source);

        auto left  = source.subseq(co{0, l});
        auto right = source.subseq(oo{r, -1});
        auto len   = source.length() - (r - l + 1);
        inc::copy_with_operator(target, left, left.length());
        inc::copy_with_operator(target.subseq(co{l, -1}), right, right.length());
        return len;
    }

    #undef xgen

    template<inc::can_unified_seqlize seq_t, can_interval interval_t>
    inline uxx remove(seq_t const & target, interval_t const & range){
        return remove_core(inc::unified_seq<seq_t>(target), range);
    }

    template<inc::can_unified_seqlize seq_t>
    inline uxx remove(seq_t const & target, ixx i){
        xindex_rollback(target.length(), i);
        return remove_core(inc::unified_seq<seq_t>(target), cc{i, i});
    }

    template<inc::can_unified_seqlize seq_tar_t, inc::can_unified_seqlize seq_src_t, can_interval interval_t>
    inline uxx remove(seq_tar_t const & target, seq_src_t const & source, interval_t const & range){
        return remove_core(
            inc::unified_seq<seq_tar_t>(target), 
            inc::unified_seq<seq_src_t>(source), 
            range
        );
    }

    template<inc::can_unified_seqlize seq_tar_t, inc::can_unified_seqlize seq_src_t>
    inline uxx remove(seq_tar_t const & target, seq_src_t const & source, ixx i){
        xindex_rollback(source.length(), i);

        return remove_core(
            inc::unified_seq<seq_tar_t>(target), 
            inc::unified_seq<seq_src_t>(source), 
            cc{i, i}
        );
    }
}

#endif

xexport(mixc::algo_remove::remove)
