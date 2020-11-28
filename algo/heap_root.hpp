/* 模块：heap_root
 * 类型：函数单体
 * 功能：用于构成大小根堆
 * 用法：
 * TODO ===========================================================
 * 
 * 注意：
 * - 函数内部抹掉了序列元素的类型
 *   内部把元素当做无格式的内存来处理可能会在多线程中导致线程安全问题
 *   类似智能指针这样带原子操作运算符重载的对象在该函数内部失去原子性
 */

#ifndef xpack_algo_heap_root
#define xpack_algo_heap_root
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_heap_root::inc
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xnew.hpp"
#include"memop/addressof.hpp"
#include"memop/copy.hpp"
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
     */
    xheader inline void push_core(seq_t seq, inc::item_origin_of<seq_t> const & value, cmp_t const & compare){
        // 注意：
        // [value] 可能存在 [seq] 中, 
        // 所以需要一个 [value] 的副本
        inc::mirror<item_t> similarities{value, inc::construction_t::ignore};
        uxx                 i = seq.length();
        uxx                 ii;

        for (; i > 0; i = ii){
            auto & parent = seq[
                ii = (i - 1) >> 1
            ];

            if (compare(parent, similarities) <= 0){
                break;
            }

            // 避免拷贝赋值
            inc::copy(xref seq[i], parent);
        }

        // 这里需要拷贝构造
        xnew (xref seq[i]) item_t(similarities);
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - insert_value 为要插入的元素，通常是当前长度 seq 序列中的最后一个元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     */
    xheader inline auto pop_core(seq_t seq, inc::item_origin_of<seq_t> const & insert_value, cmp_t const &  compare){
        // 避免返回值的复制构造
        auto && wanted     = inc::transmitter{ seq[0] };
        uxx     i          = 0;
        uxx     left_index = 1;

        while(left_index + 1 < seq.length()) {
            item_t & left  = seq[left_index];
            item_t & right = seq[left_index + 1];
            item_t * select;

            // 小根堆 父节点要小于左右节点 所以要找子节点中较小者
            if (compare(left, right) > 0){
                select = xref right;
                left_index++;
            }
            else{
                select = xref left;
            }

            // 较小的子节点大于等于插入 就不用再下移了
            if (compare(select[0], insert_value) >= 0){
                break;
            }

            inc::copy(xref seq[i], select[0]);
            i          = (left_index);
            left_index = (left_index << 1) + 1;
        }

        xnew (xref seq[i]) item_t(insert_value);
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
     * - insert_value 为要插入的元素，通常是当前长度 seq 序列中的最后一个元素
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     */
    xheader inline auto pop(
        seq_t                       const & seq, 
        uxx                                 length, 
        inc::item_origin_of<seq_t>  const & insert_value, 
        cmp_t                       const & compare = inc::default_compare<item_t>){

        return pop_core(
            inc::unified_seq<seq_t>{seq}.subseq(co{0, length}), 
            insert_value, 
            compare
        );
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 can_unified_seqlize 约束的序列类型
     * - compare 为元素比较回调，期望签名如下：
     *      ixx operator()(item_t const & left, item_t const & right)
     *   其中 item_t 是 seq 序列元素的类型，left 和 right 作为 seq 序列中两两比较的元素
     *   当 left 大于 right 返回正数，若小于则返回负数，相等则返回零
     */
    xheader inline auto pop(
        seq_t                       const & seq, 
        uxx                                 length, 
        cmp_t                       const & compare = inc::default_compare<item_t>){

        inc::unified_seq<seq_t> v{seq};
        return pop_core(
            v.subseq(co{0, length}), 
            v[length - 1], 
            compare
        );
    }
}

#endif

xexport_space(mixc::algo_heap_root::origin)
