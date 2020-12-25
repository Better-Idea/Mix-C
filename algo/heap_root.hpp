/* 模块：heap_root
 * 类型：函数单体
 * 功能：用于构成大小根堆
 */

#ifndef xpack_algo_heap_root
#define xpack_algo_heap_root
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_heap_root::inc
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xindex_rollback.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_heap_root{
    #define xheader                                                                 \
    template<                                                                       \
        inc::can_unified_seqlize seq_t,                                             \
        class                    item_t  = inc::item_origin_of<seq_t>,              \
        class                    cmp_t   = decltype(inc::default_compare<item_t>)   \
    >                                                                               \
    requires(                                                                       \
        inc::can_compare<cmp_t, item_t> == true                                     \
    )

    /* 函数：大小根堆压栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - value 为要压栈的元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     * 注意：
     * 我们假定 seq [0, length) 区间的元素已经完成了初始化
     * 或者 seq 的元素类似与 int 这样不初始化也可以直接赋值的类型
     * 如果 seq[i] 带有 operator=，并且它没有初始化，那么调用该函数将导致不正确的行为
     */
    xheader inline void push_core(
        seq_t                               seq, 
        inc::item_origin_of<seq_t> const &  value, 
        cmp_t const &                       compare){

        // 注意：
        // [value] 可能存在 [seq] 中, 
        // 所以需要一个 [value] 的副本
        item_t insert       = value;
        uxx    i            = seq.length();
        uxx    ii;

        for (; i > 0; i = ii){
            auto & parent   = seq[
                ii          = (i - 1) >> 1
            ];

            if (compare(parent, insert) <= 0){
                break;
            }

            seq[i]          = parent;
        }
        seq[i]              = insert;
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - insert_value 为要插入的元素，通常是当前长度 seq 序列中的最后一个元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     * 注意：
     * 我们假定 seq [0, length) 区间的元素已经完成了初始化
     * 或者 seq 的元素类似与 int 这样不初始化也可以直接赋值的类型
     * 如果 seq[i] 带有 operator=，并且它没有初始化，那么调用该函数将导致不正确的行为
     */
    xheader inline item_t pop_core(
        seq_t                               seq, 
        uxx                                 index, 
        inc::item_origin_of<seq_t> const &  insert_value, 
        cmp_t                      const &  compare){

        // 注意：
        // 提前保存要取出元素的副本，避免后续的元素覆盖它
        item_t  wanted      = (seq[index]);
        uxx     i           = (index);
        uxx     left_index  = (index << 1) + 1;
        uxx     len         = seq.length();

        while(left_index + 1 < len) {
            item_t & left   = seq[left_index];
            item_t & right  = seq[left_index + 1];
            item_t * select;

            // 小根堆 父节点要小于左右节点 所以要找子节点中较小者
            if (compare(left, right) > 0){
                select      = xref right;
                left_index++;
            }
            else{
                select      = xref left;
            }

            // 较小的子节点大于等于插入 就不用再下移了
            if (compare(select[0], insert_value) >= 0){
                break;
            }

            seq[i]          = (select[0]);
            i               = (left_index);
            left_index      = (left_index << 1) + 1;
        }

        // left_index 可能会等于 len，所以这里需要判断一下才能确认是否存在左节点
        if (left_index < len and compare(seq[left_index], insert_value) < 0){
            seq[i]          = (seq[left_index]);
            i               = (left_index);
        }

        seq[i]              = insert_value;
        return wanted;
    }
}

namespace mixc::algo_heap_root::origin{
    using inc::default_compare_neg;
}

namespace mixc::algo_heap_root::origin::heap_root{
    /* 函数：大小根堆压栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - length 为当前 seq 元素个数
     * - value 为要压栈的元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     * 注意：
     * 我们假定 seq [0, length) 区间的元素已经完成了初始化
     * 或者 seq 的元素类似与 int 这样不初始化也可以直接赋值的类型
     * 如果 seq[i] 带有 operator=，并且它没有初始化，那么调用该函数将导致不正确的行为
     */
    xheader inline void push(
        seq_t                       const & seq,
        uxx                                 length,
        inc::item_origin_of<seq_t>  const & value, 
        cmp_t                       const & compare = inc::default_compare<item_t>){

        push_core(
            inc::unified_seq<seq_t>{seq}.subseq(co{0, length}), 
            value, 
            compare
        );
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - length 为 seq 序列的长度
     * - insert_value 为要插入的元素，通常是当前长度 seq 序列中的最后一个元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     * 注意：
     * 我们假定 seq [0, length) 区间的元素已经完成了初始化
     * 或者 seq 的元素类似与 int 这样不初始化也可以直接赋值的类型
     * 如果 seq[i] 带有 operator=，并且它没有初始化，那么调用该函数将导致不正确的行为
     */
    xheader inline auto pop(
        seq_t                       const & seq, 
        uxx                                 length, 
        inc::item_origin_of<seq_t>  const & insert_value, 
        cmp_t                       const & compare = inc::default_compare<item_t>){

        return pop_core(
            inc::unified_seq<seq_t>{seq}.subseq(co{0, length}), 
            0,
            insert_value, 
            compare
        );
    }

    /* 函数：大小根堆弹栈操作，取出指定索引的元素
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - length 为 seq 序列的长度
     * - index 为要取出元素对应的索引
     * - insert_value 为要插入的元素，通常是当前长度 seq 序列中的最后一个元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     * 注意：
     * 我们假定 seq [0, length) 区间的元素已经完成了初始化
     * 或者 seq 的元素类似与 int 这样不初始化也可以直接赋值的类型
     * 如果 seq[i] 带有 operator=，并且它没有初始化，那么调用该函数将导致不正确的行为
     */
    xheader inline auto pop_by_index(
        seq_t                       const & seq, 
        uxx                                 length, 
        ixx                                 index,
        inc::item_origin_of<seq_t>  const & insert_value, 
        cmp_t                       const & compare = inc::default_compare<item_t>){

        xindex_rollback(length, index);
        return pop_core(
            inc::unified_seq<seq_t>{seq}.subseq(co{0, length}), 
            index,
            insert_value, 
            compare
        );
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - length 为 seq 序列的长度
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     * 注意：
     * 我们假定 seq [0, length) 区间的元素已经完成了初始化
     * 或者 seq 的元素类似与 int 这样不初始化也可以直接赋值的类型
     * 如果 seq[i] 带有 operator=，并且它没有初始化，那么调用该函数将导致不正确的行为
     */
    xheader inline auto pop(
        seq_t                       const & seq, 
        uxx                                 length, 
        cmp_t                       const & compare = inc::default_compare<item_t>){

        inc::unified_seq<seq_t> v{seq};
        return pop_core(
            v.subseq(co{0, length}), 0,
            v[length - 1], 
            compare
        );
    }

    /* 函数：大小根堆弹栈操作，取出指定索引的元素
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - length 为 seq 序列的长度
     * - index 为要取出元素对应的索引
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     */
    xheader inline auto pop_by_index(
        seq_t                       const & seq, 
        uxx                                 length, 
        ixx                                 index,
        cmp_t                       const & compare = inc::default_compare<item_t>){

        xindex_rollback(length, index);
        inc::unified_seq<seq_t> v{seq};
        return pop_core(
            v.subseq(co{0, length}), index,
            v[length - 1], 
            compare
        );
    }
}

#endif

xexport_space(mixc::algo_heap_root::origin)
