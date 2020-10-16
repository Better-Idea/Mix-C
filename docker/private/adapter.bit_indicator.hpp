/* 模块：adapter_bit_indicator
 * 类型：适配器
 * 功能：通过将栈/堆 bit_indicator 提供的数据，对外提供位指示器的一系列基础功能
 * 用法：
 * TODO======================================================================================
 */

#ifndef xpack_docker_adapter_bit_indicator
#define xpack_docker_adapter_bit_indicator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_adapter_bit_indicator
#include"instruction/index_of_first_set.hpp"
#include"instruction/index_of_last_set.hpp"
#include"interface/unified_seq.hpp"
#include"memop/zeros.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_adapter_bit_indicator::origin{
    static constexpr uxx step_exp = xis_os64 ? 6 : 5;
    static constexpr uxx bwidth   = 1u << step_exp;
    static constexpr uxx bmask    = bwidth - 1;

    /* 函数：设置一个位图树中对应位的状态
     * 参数：
     * - index 为目标操作位的索引
     * - bmp 为位图内存首地址
     * - level_lut 为位图树每一级有多少个 uxx 字
     * - height 为位图树的高度
     * - invoke 为位操作回调，期望签名如下：
     *      bool invoke(uxx * current, uxx index)
     * 补充：
     * - invoke 可能时复位或者置位操作
     * - 从位图树的底层向上层操作
     * - 如果 invoke 复位操作，且在复位后发现当前节点还有其他置位位，则无需再往上级操作
     * - 如果 invoke 置位操作，且在置位前发现当前节点无其他置位位，则无需再往上级操作
     */
    template<class callback>
    inline void bmp_set(uxx index, uxx * bmp, const uxx * level_lut, uxx height, callback invoke){
        uxx i = 0;
        uxx j = 0;

        while(true){
            if (j = index >> step_exp; invoke(xref bmp[j], uxx(1) << (index & bmask))){
                break;
            }
            if (i + 1 == height){
                break;
            }

            bmp    += level_lut[i];
            i      += 1;
            index   = j;
        }
    }

    /* 函数：在位图树中按指定方式进行位寻找置位位
     * 参数：
     * - bmp 为位图树内存首地址
     * - length 为位图树总长度
     * - level_lut 为位图树每一级有多少个 uxx 字
     * - height 为位图树的高度
     * - invoke 为位操作回调，期望签名如下：
     *      uxx invoke(uxx word)
     * 返回：
     * - 满足指定搜索方式的置位位在位图中的索引
     * - 若未找到则返回 not_exist
     */
    template<class callback>
    inline uxx bmp_find(uxx * bmp, uxx length, const uxx * level_lut, uxx height, callback invoke){
        uxx  offset = 0;
        uxx  index  = 0;
        uxxp p      = bmp + length;

        for(uxx i = height; i-- > 0;){
            p      -= level_lut[i];
            offset  = invoke(p[index]);

            if (offset == not_exist){
                return not_exist;
            }
            index   = index << step_exp | offset;
        }
        return index;
    }

    static constexpr bool is_find_last = true;

    /* 函数：约束边界，在位图树中按指定方式进行位寻找置位位
     * 参数：
     * - bmp 为位图树内存首地址
     * - level_lut 为位图树每一级有多少个 uxx 字
     * - height 为位图树的高度
     * - invoke 为位操作回调，期望签名如下：
     *      uxx invoke(uxx word)
     * - boundary 为索引的边界，搜寻范围不超过此边界
     * 返回：
     * - 满足指定搜索方式的置位位在位图中的索引
     * - 若未找到则返回 not_exist
     */
    template<class callback>
    inline uxx bmp_find(bool is_find_last, uxx * bmp, const uxx * level_lut, uxx height, callback invoke, uxx boundary){
        uxx  w      = boundary >> step_exp;

        if (not is_find_last and w >= level_lut[0]){
            return not_exist;
        }

        uxx  i      = 0;
        uxx  mask   = is_find_last ? uxx(-1) >> (bmask - (boundary & bmask)) : uxx(-1) << (boundary & bmask);
        uxx  r      = invoke(bmp[w] & mask);

        if (r != not_exist){
            return w << step_exp | r;
        }

        while(i < height - 1){
            // 从下向上
            bmp    += level_lut[i];
            mask    = is_find_last ? uxx(-1) >> (bwidth - (w & bmask)) : uxx(-1) << ((w & bmask) + 1);
            w     >>= step_exp;
            r       = invoke(bmp[w] & mask);

            if (r == not_exist){
                i  += 1;
                continue;
            }

            // 从上向下
            do{
                bmp -= level_lut[i];
                w    = w << step_exp | r;
                r    = invoke(bmp[w]);
            } while(i-- != 0);

            return w << step_exp | r;
        }
        return not_exist;
    }
    
    template<class base_t>
    /*
    needed:
    - base_t::bmp()
    - base_t::size()
    - base_t::level_lut()
    - base_t::height()
    */
    xstruct(
        xname(adapter_bit_indicator_t),
        xpubb(base_t)
    )
        using base_t::base_t;
    protected:
        void clear(){
            inc::zeros(base_t::bmp(), base_t::size() * sizeof(uxx));
        }

        void set(uxx index){
            bmp_set(index, base_t::bmp(), base_t::level_lut(), base_t::height(), [](uxx * p, uxx val){
                bool has_set = *p != 0;
                *p          |= val;
                return has_set;
            });
        }

        template<class seq_t>
        void bulk_set(seq_t const & index_list){
            for(uxx i = 0; i < index_list.length(); i++){
                set(index_list[i]);
            }
        }

        void reset(uxx index){
            bmp_set(index, base_t::bmp(), base_t::level_lut(), base_t::height(), [](uxx * p, uxx val){
                *p &= ~val;
                return *p != 0;
            });
        }

        template<class seq_t>
        void bulk_reset(seq_t const & index_list){
            for(uxx i = 0; i < index_list.length(); i++){
                reset(index_list[i]);
            }
        }
    public:
        /* 函数：得到指定索引的位状态
         * 参数：
         * - index 目标索引
         */
        bool get(uxx index){
            auto i  = index >> step_exp;
            auto m  = uxx(1) << (index & bmask);
            return (base_t::bmp()[i] & m) != 0;
        }

        /* 函数：得到第一个置位位索引并对它复位 */
        uxx pop_first(){
            if (uxx index = index_of_first_set(); index != not_exist){
                reset(index);
                return index;
            }
            return not_exist;
        }

        /* 函数：得到最后一个置位位索引并对它复位 */
        uxx pop_last(){
            if (uxx index = index_of_last_set(); index != not_exist){
                reset(index);
                return index;
            }
            return not_exist;
        }

        /* 函数：得到第一个置位位索引 */
        uxx index_of_first_set(){
            return bmp_find(
                base_t::bmp(),
                base_t::size(),
                base_t::level_lut(),
                base_t::height(),
                &  inc::index_of_first_set<uxx>
            );
        }

        /* 函数：从指定索引开始寻找第一个置位位索引
         * 参数：
         * - start_boundary 为搜索的起始索引
         */
        uxx index_of_first_set(uxx start_boundary){
            return bmp_find(
                not is_find_last,
                base_t::bmp(),
                base_t::level_lut(),
                base_t::height(),
                &  inc::index_of_first_set<uxx>, start_boundary
            );
        }

        /* 函数：得到最后一个置位位索引 */
        uxx index_of_last_set(){
            return bmp_find(
                base_t::bmp(),
                base_t::size(),
                base_t::level_lut(),
                base_t::height(),
                &  inc::index_of_last_set<uxx>
            );
        }

        /* 函数：从指定索引开始寻找最后一个置位位索引
         * 参数：
         * - end_boundary 为搜索的起始索引，从后往前搜索
         */
        uxx index_of_last_set(uxx end_boundary){
            return bmp_find(
                is_find_last,
                base_t::bmp(),
                base_t::level_lut(),
                base_t::height(),
                &  inc::index_of_last_set<uxx>, end_boundary
            );
        }
    $

    template<class final, class base>
    struct adapter_bit_indicator : adapter_bit_indicator_t<base>{
        using the_t = adapter_bit_indicator_t<base>;
        using the_t::the_t;
        using the_t::pop_first;
        using the_t::pop_last;
        using the_t::index_of_first_set;
        using the_t::index_of_last_set;
        using the_t::get;

        /* 函数：得到第一个置位位索引并对它复位
         * 参数：
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & pop_first(uxx * value){
            value[0] = pop_first();
            return thex;
        }

        /* 函数：得到最后一个置位位索引并对它复位
         * 参数：
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & pop_last(uxx * value){
            value[0] = pop_last();
            return thex;
        }

        
        /* 函数：得到第一个置位位索引
         * 参数：
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & index_of_first_set(uxx * value){
            value[0] = index_of_first_set();
            return thex;
        }

        /* 函数：从指定索引开始寻找第一个置位位索引
         * 参数：
         * - start_boundary 为搜索的起始索引
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & index_of_first_set(uxx start_boundary, uxx * value){
            value[0] = index_of_first_set(start_boundary);
            return thex;
        }

        /* 函数：得到最后一个置位位索引
         * 参数：
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & index_of_last_set(uxx * value){
            value[0] = index_of_last_set();
            return thex;
        }

        /* 函数：从指定索引开始寻找最后一个置位位索引
         * 参数：
         * - end_boundary 为搜索的起始索引，从后往前搜索
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & index_of_last_set(uxx end_boundary, uxx * value){
            value[0] = index_of_last_set(end_boundary);
            return thex;
        }

        /* 函数：得到指定索引的位状态
         * 参数：
         * - index 目标索引
         * - value 为接收结果的内存
         * 返回：
         * - 当前对象的引用
         */
        final & get(uxx index, bool * value){
            value[0] = get(index);
            return thex;
        }

        /* 函数：将对指定索引的位置位
         * 参数：
         * - index 目标索引
         * 返回：
         * - 当前对象的引用
         */
        final & set(uxx index){
            the.set(index);
            return thex;
        }

        /* 函数：将对指定索引的位设置
         * 参数：
         * - index 目标索引
         * - value 要设置的状态
         * 返回：
         * - 当前对象的引用
         */
        final & set(uxx index, bool value){
            value ? the.set(index) : the.reset(index);
            return thex;
        }

        /* 函数：通过索引数组将对依次对指定索引的位置位
         * 参数：
         * - index_group 目标索引序列
         * 返回：
         * - 当前对象的引用
         */
        template<inc::unified_seq_t seq_t>
        final & set(seq_t const & index_group){
            the.bulk_set(inc::unified_seq<seq_t>{index_group});
            return thex;
        }

        /* 函数：通过索引数组将对依次对指定索引的位置位
         * 参数：
         * - index_group 目标索引序列
         * - value 要设置的状态
         * 返回：
         * - 当前对象的引用
         */
        template<inc::unified_seq_t seq_t>
        final & set(seq_t const & index_group, bool value){
            value ? the.bulk_set(inc::unified_seq<seq_t>{index_group}) :
                the.bulk_reset(inc::unified_seq<seq_t>{index_group});
            return thex;
        }

        /* 函数：将对指定索引的位复位
         * 参数：
         * - index 目标索引
         * 返回：
         * - 当前对象的引用
         */
        final & reset(uxx index){
            the.reset(index);
            return thex;
        }

        /* 函数：通过索引数组将对依次对指定索引的位复位
         * 参数：
         * - index_group 目标索引序列
         * 返回：
         * - 当前对象的引用
         */
        template<inc::unified_seq_t seq_t>
        final & reset(seq_t const & index_group){
            the.bulk_reset(inc::unified_seq<seq_t>{index_group});
            return thex;
        }
    };
}

#endif

xexport_space(mixc::docker_adapter_bit_indicator::origin)
