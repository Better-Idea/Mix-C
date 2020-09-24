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
#define xuser mixc::algo_heap_root
#include"define/base_type.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"interface/can_compare.hpp"
#include"interface/unified_seq.hpp"
#include"memop/addressof.hpp"
#include"memop/copy.hpp"
#include"memory/new.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_heap_root{
    /* 函数：大小根堆压栈操作
     * 参数：
     * - seq 为满足 inc::unified_seq_t 约束的序列类型
     * - value 为要压栈的元素
     * - compare 为元素比较回调
     */
    template<class item_t, inc::unified_seq_t seq_t>
    inline void push_core(
        seq_t                       seq,
        item_t const &              value, 
        inc::can_compare<item_t>    compare = inc::default_compare<item_t>){
        
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
        new (xref seq[i]) item_t(similarities);
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 inc::unified_seq_t 约束的序列类型
     * - insert_value 为要插入的元素
     * - compare 为元素比较回调
     */
    template<class item_t, inc::unified_seq_t seq_t>
    inline auto pop_core(
        seq_t                       seq, 
        item_t const &              insert_value, 
        inc::can_compare<item_t>    compare = inc::default_compare<item_t>){

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

        new (xref seq[i]) item_t(insert_value);
        return wanted;
    }
}

namespace mixc::algo_heap_root::origin::heap_root{
    /* 函数：大小根堆压栈操作
     * 参数：
     * - seq 为满足 inc::unified_seq_t 约束的序列类型
     * - length 为序列长度
     * - value 为要压栈的元素
     * - compare 为元素比较回调
     */
    template<inc::unified_seq_t seq_t>
    inline void push(
        seq_t                       const & seq,
        uxx                                 length,
        inc::item_origin_of<seq_t>  const & value, 
        inc::can_compare<
            inc::item_origin_of<seq_t>
        >                                   compare = 
            inc::default_compare<
                inc::item_origin_of<seq_t>
            >){
        push_core<
            inc::item_origin_of<seq_t>
        >(inc::unified_seq<seq_t>(seq).subseq(co{0, length}), value, compare);
    }

    /* 函数：大小根堆弹栈操作
     * 参数：
     * - seq 为满足 inc::unified_seq_t 约束的序列类型
     * - length 为序列长度
     * - insert_value 为要插入的元素
     * - compare 为元素比较回调
     */
    template<inc::unified_seq_t seq_t>
    inline auto pop(
        seq_t                       const & seq, 
        uxx                                 length, 
        inc::item_origin_of<seq_t>  const & insert_value, 
        inc::can_compare<
            inc::item_origin_of<seq_t>
        >                                   compare = 
            inc::default_compare<
                inc::item_origin_of<seq_t>
            >){
        return pop_core<
            inc::item_origin_of<seq_t>
        >(inc::unified_seq<seq_t>(seq).subseq(co{0, length}), insert_value, compare);
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::algo_heap_root::origin;
}
