#ifndef xpack_algo_insert
#define xpack_algo_insert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_insert::inc
#include"interface/seqptr.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xindex_rollback.hpp"
#include"memop/copy.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_insert{
    template<class seq_des_t, class seq_src_t = seq_des_t>
    inline void insert_core(
        seq_des_t                   target, 
        ixx                         index, 
        seq_src_t                   values){
        
        using item_t = inc::item_origin_of<seq_des_t>;
        xindex_rollback(target.length(), index, +1);

        ixx i   = ixx(target.length());
        ixx j   = ixx(values.length());

        while(i > index){
            i            -= 1;
            target[i + j] = target[i];
        }
        for(i = 0; i < j; i++){
            target[i + index] = (item_t)values[i];
        }
    }

    template<class seq_des_t, class seq_src_t = seq_des_t, class seq_val_t = seq_src_t>
    inline void insert_core(
        seq_des_t                           target,
        seq_src_t                           source,
        ixx                                 index,
        seq_val_t                           values){

        xindex_rollback(target.length(), index, +1);
        auto tar   = target.subseq(co{index});
        inc::copy_with_operator(target, source, index);
        inc::copy_with_operator(tar, values, values.length());
        inc::copy_with_operator(
            tar.subseq(co{values.length()}), 
            source.subseq(co{index}), 
            source.length() - index
        );
    }

    template<inc::unified_seq_t seq_t>
    inline void insert(
        seq_t                       const & target,
        ixx                                 index,
        inc::item_origin_of<seq_t>  const & value){

        using item_t = inc::item_origin_of<seq_t>;

        insert_core(
            inc::unified_seq<seq_t>(target), index, 
            inc::seqptr<item_t>{value}
        );
    }

    template<inc::unified_seq_t seq_des_t, inc::unified_seq_t seq_val_t>
    inline void insert(
        seq_des_t                   const & target,
        ixx                                 index,
        seq_val_t                   const & values){

        insert_core(
            inc::unified_seq<seq_des_t>(target), index, 
            inc::unified_seq<seq_val_t>(values)
        );
    }

    template<inc::unified_seq_t seq_tar_t, inc::unified_seq_t seq_src_t, inc::unified_seq_t seq_val_t>
    inline void insert(
        seq_tar_t                   const & target,
        seq_src_t                   const & source,
        ixx                                 index,
        seq_val_t                   const & values){

        insert_core(
            inc::unified_seq<seq_tar_t>(target), 
            inc::unified_seq<seq_src_t>(source),  index, 
            inc::unified_seq<seq_val_t>(values)
        );
    }

    template<inc::unified_seq_t seq_tar_t, inc::unified_seq_t seq_src_t, class item_t>
    inline void insert(
        seq_tar_t                   const & target,
        seq_src_t                   const & source,
        ixx                                 index,
        item_t                      const & value){

        insert_core(
            inc::unified_seq<seq_tar_t>(target), 
            inc::unified_seq<seq_src_t>(source),  index, 
            inc::seqptr<item_t>{value}
        );
    }
}

#endif

xexport(mixc::algo_insert::insert)
