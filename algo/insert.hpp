/* 模块：insert
 * 类型：函数单体
 * 功能：在一个序列中插入新的元素
 * 用法：
 * TODO ===========================================================
 * 
 * 注意：
 * TODO ===========================================================
 */

#ifndef xpack_algo_insert
#define xpack_algo_insert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_insert::inc
#include"interface/seqptr.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xindex_rollback.hpp"
#include"memop/copy.hpp"
#include"meta/has_cast.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_insert{
    /* 函数：序列插入新元素
     * 参数：
     * - target 为待插入新元素的序列，并满足 unified_seq<T> 约束
     * - index 为新元素要插入的索引，支持负数索引（-1 表示插入到末尾，-2 表示倒数第一个元素的位置，依此类推）
     * - values 为要插入的元素序列，并满足 unified_seq<T> 约束
     * 返回：
     * 插入元素后序列的长度
     */
    template<class seq_tar_t, class seq_src_t>
    inline uxx insert_core(
        seq_tar_t                   target, 
        ixx                         index, 
        seq_src_t                   values){
        
        using item_t = inc::item_origin_of<seq_tar_t>;
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
        return target.length() + values.length();
    }

    /* 函数：序列插入新元素
     * 参数：
     * - target 为结果缓冲区，并满足 unified_seq<T> 约束
     * - source 为源数据序列，并满足 unified_seq<T> 约束
     * - index 为新元素要插入的索引，支持负数索引（-1 表示插入到末尾，-2 表示倒数第一个元素的位置，依此类推）
     * - values 为要插入的元素序列，并满足 unified_seq<T> 约束
     * 返回：
     * 插入元素后序列的长度
     */
    template<class seq_tar_t, class seq_src_t, class seq_val_t>
    inline uxx insert_core(
        seq_tar_t                           target,
        seq_src_t                           source,
        ixx                                 index,
        seq_val_t                           values){

        xindex_rollback(source.length(), index, +1);
        auto tar   = target.subseq(co{index});
        inc::copy_with_operator(target, source, index);
        inc::copy_with_operator(tar, values, values.length());
        inc::copy_with_operator(
            tar.subseq(co{values.length()}), 
            source.subseq(co{index}), 
            source.length() - index
        );
        return source.length() + values.length();
    }

    /* 函数：序列插入新元素
     * 参数：
     * - target 为待插入新元素的序列，并满足 can_unified_seqlize 约束
     * - index 为新元素要插入的索引，支持负数索引（-1 表示插入到末尾，-2 表示倒数第一个元素的位置，依此类推）
     * - value 为要插入的元素，类型需要和 target 元素类型保持一致
     * 返回：
     * 插入元素后序列的长度
     */
    template<inc::can_unified_seqlize seq_t>
    inline uxx insert(
        seq_t                       const & target,
        ixx                                 index,
        inc::item_origin_of<seq_t>  const & value){

        using item_t = inc::item_origin_of<seq_t>;

        return insert_core(
            inc::unified_seq<seq_t>(target), index, 
            inc::seqptr<item_t>({ value })
        );
    }

    /* 函数：序列插入新元素
     * 参数：
     * - target 为待插入新元素的序列，并满足 can_unified_seqlize 约束
     * - index 为新元素要插入的索引，支持负数索引（-1 表示插入到末尾，-2 表示倒数第一个元素的位置，依此类推）
     * - values 为要插入的元素序列，并满足 can_unified_seqlize 约束
     * 返回：
     * 插入元素后序列的长度
     */
    template<
        inc::can_unified_seqlize seq_tar_t,
        class                    seq_val_t = inc::seqptr<
            inc::item_origin_of<seq_tar_t>
        >
    >
    requires(inc::can_seqptrlize<seq_val_t>)
    inline uxx insert(
        seq_tar_t                   const & target,
        ixx                                 index,
        seq_val_t                   const & values){

        return insert_core(
            inc::unified_seq<seq_tar_t>(target), index, 
            inc::unified_seq<seq_val_t>(values)
        );
    }

    /* 函数：序列插入新元素
     * 参数：
     * - target 为结果缓冲区，并满足 can_unified_seqlize 约束
     * - source 为源数据序列，并满足 can_unified_seqlize 约束
     * - index 为新元素要插入的索引，支持负数索引（-1 表示插入到末尾，-2 表示倒数第一个元素的位置，依此类推）
     * - values 为要插入的元素序列，并满足 can_unified_seqlize 约束
     * 返回：
     * 插入元素后序列的长度
     */
    template<
        inc::can_unified_seqlize    seq_tar_t, 
        class                       seq_src_t = inc::seqptr<
            inc::item_origin_of<seq_tar_t>
        >,
        class                       seq_val_t = seq_src_t
    >
    requires(
        inc::has_cast<
            inc::item_origin_of<seq_tar_t>,
            inc::item_origin_of<seq_src_t>
        > == true and
        inc::has_cast<
            inc::item_origin_of<seq_tar_t>,
            inc::item_origin_of<seq_val_t>
        > == true and
        inc::has_cast<
            inc::item_origin_of<seq_src_t>,
            inc::item_origin_of<seq_val_t>
        > == true 
    )
    inline uxx insert(
        seq_tar_t                   const & target,
        seq_src_t                   const & source,
        ixx                                 index,
        seq_val_t                   const & values){

        return insert_core(
            inc::unified_seq<seq_tar_t>(target), 
            inc::unified_seq<seq_src_t>(source),  index, 
            inc::unified_seq<seq_val_t>(values)
        );
    }

    /* 函数：序列插入新元素
     * 参数：
     * - target 为结果缓冲区，并满足 can_unified_seqlize 约束
     * - source 为源数据序列，并满足 can_unified_seqlize 约束
     * - index 为新元素要插入的索引，支持负数索引（-1 表示插入到末尾，-2 表示倒数第一个元素的位置，依此类推）
     * - value 为要插入的元素，类型和 target 的元素类型保持一致
     * 返回：
     * 插入元素后序列的长度
     */
    template<
        inc::can_unified_seqlize    seq_tar_t, 
        class                       item_t    = inc::item_origin_of<seq_tar_t>,
        class                       seq_src_t = inc::seqptr<item_t>
    >
    inline uxx insert(
        seq_tar_t                   const & target,
        seq_src_t                   const & source,
        ixx                                 index,
        inc::item_origin_of<seq_tar_t>
                                    const & value){

        return insert_core(
            inc::unified_seq<seq_tar_t>(target), 
            inc::unified_seq<seq_src_t>(source),  index, 
            inc::seqptr<item_t>({value})
        );
    }
}

#endif

xexport(mixc::algo_insert::insert)
