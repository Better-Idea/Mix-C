/* 现代 MMU 概念
 * 面向：
 * - 管理超过 1TB 物理内存
 * - 最多约 2G 个显式对象
 * - 更浅的页表层次
 * - 内存的访问以对象句柄为媒介
 * 
 * 显式对象：
 * - 等效的指针大小为 4Byte
 * - 由系统直接管理的对象
 * 
 * 隐式对象：
 * - 等效的指针大小为 8Byte
 * - 以显式对象为基址 4Byte + 4Byte 偏移组成的对象
 * - 不能单独释放该对象指向的内存
 * - 伴随指向的显式对象的释放而无效
 * 
 * 分区：
 * - 大小 16MB
 * - 别名为大页
 * 
 * 混合区：
 * - 内存低 8GB 地址空间
 * - 包含对象分区和数据分区
 * - 通过位指示器(位图)区分空闲/非空闲、数据/对象分区
 * - 主要存放对象分区，当对象分区耗尽时则会将其中的数据分区整理、迁移以便腾出内存用于存放对象句柄
 * - 当混合区存满了对象分区时将产生溢出中断，我们把处理该事务的职责交给系统开发人员
 * 
 * 纯数据区：
 * - 高于 8GB 的地址空间
 * - 只包含数据分区
 * - 优先从该分区分配内存填充对象句柄
 * 
 * 对象分区：
 * - 大小 16MB
 * - 不可移动
 * - 位于低 8GB 地址空间
 * - 最多容纳约 2G 个对象
 * - 包含 16K 个 1KB 的对象组
 * 
 * 数据分区：
 * - 大小 16MB
 * - 可移动、整理
 * - 可拆成 2 的次幂大小的小页
 * - 拆分的小页在分配时需要在首部记录 4Byte 的所属对象，16MB 的大页则不需要加
 * 
 * 对象组：
 * - 大小 1KB
 * - 包含 16Byte 对象组首部
 *     - 通过所属进程 id + 线程 id 保护/共享内存
 *     - 包含小页(对象组专用)的高 24 位作为对象组内成员的公共段地址
 * - 包含 252 * 4Byte 对象
 * - 当与之绑定的小页内存分配完毕时，该对象组将不能再分配对象
 * - 只有当小页不能分配所需大小的内存时，才能为该对象组所属的线程分配新的对象组
 * 
 * 对象组专用小页：
 * - 大小 64KB
 * - 满足 数据分区 中的小页说明，但该小页的所属对象指向的时对象组的首部(可以用于与其他通用小页区分)
 * - 可以被多个对象组绑定，但这些对象组必须属于同一个线程
 * - 包含对象组的对象分配与回收功能
 * 
 * 对象：
 * - 只读/读写 + 可执行类型设置
 * - 固定大小/动态增长
 * - 层次结构
 *     - 0. 分配小于 4Byte 时：直接访问该对象包含的 3 个可用的字节
 *     - 1. 分配小于   1KB 时：通过结合该对象所在对象组中的段地址 + 该对象指向偏移构成的物理地址访问内存
 *     - 2. 分配不超过 4GB 时：通过 1. 得到该对象指向的页表，再经过一级地址变换后得到的物理地址访问内存
 *     - 3. 不超过物理大小 时：联合多个 4GB 对象组合形成超大内存的对象序列，访问方式和 2. 一致
 * 
 */ 

#ifndef xpack_algo_mmu
#define xpack_algo_mmu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_mmu::inc
#include"concurrency/lock/atom_swap.hpp"
#include"dumb/move.hpp"
#include"instruction/bit_test_and_reset.hpp"
#include"instruction/index_of_last_set.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_free.hpp"
#include"macro/xnew.hpp"
#include"memop/copy.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_mmu::origin {
    struct pagoff{ uxx page, offset; };

    /* 结构：寻址器
     * 描述：让多级页表在逻辑上连续
     */
    struct addressing{
        /* 函数：可变大小的内存寻址
         * 参数：
         * - present_indicator 为页存在标志。当 2^n 大小的页存在时，第 n 位为置位态
         *   并且从第一个置位位到最后一个置位位之间必须是连续的 1
         * - address 为逻辑地址
         * 返回：
         * - 页号+偏移的组合地址
         */
        static pagoff var(uxx present_indicator, uxx address){
            uxx    mask = present_indicator & address;
            pagoff r;
            r.page   = inc::index_of_last_set(mask) + 1;
            r.offset = address;

            if (r.page != 0){
                inc::bit_test_and_reset(& r.offset, r.page);
            }
            return r;
        }

        /* 函数：固定大小的内存寻址
         * 参数：
         * - present_indicator 为页存在标志。当 2^n 大小的页存在时，第 n 位为置位态
         *   所有存在的页的加起来表示一块固定大小的内存，此后存在位不能再修改
         * - address 为逻辑地址
         * 返回：
         * - 页号+偏移的组合地址
         */
        static pagoff fixed(uxx present_indicator, uxx address){
            pagoff r;
            uxx mask = present_indicator ^ address;
            r.page   = inc::index_of_last_set(mask);
            r.offset = address;
            return r;
        }
    };

    constexpr bool with_fixed_page_table = true;

    /* 结构：可变大小数组模板
     * 参数：
     * - initial_alloc_length 为数组的起始分配长度，需要二进制对齐且大于 0
     *   只有第一次和第二次分配时，initial_alloc_length 的值不变，此后每次变为之前的两倍
     * - with_fixed_page_table 指示是否使用固定大小的页表
     * 注意：
     * - 创建数组的在未添加元素时是没有分配内存的，只有在添加元素后才会分配内存
     * - 使用 with_fixed_page_table 选项后，将不再单独为 page_table_ptr 分配和释放内存，内部假定页表是已分配好的
     * - 并且要求传入的 page_table_ptr 指向的是一个空指针数组（一个数组，里面的值都是 nullptr）
     */
    template<uxx initial_alloc_length = 1, bool with_fixed_page_table = not origin::with_fixed_page_table>
    requires(
        (initial_alloc_length & (initial_alloc_length - 1)) == 0 and initial_alloc_length > 0
    )
    struct var_array {
        /* 函数：压栈
         * 参数：
         * - page_table_ptr 为指向页表数组的指针
         * - length 为数组长度指针
         * - value 为要压栈的元素
         * - alloc 为分配回调
         * - free 为回收回调
         * 注意：
         * 要求传入的 page_table_ptr 指向的是一个空指针数组（一个数组，里面的值都是 nullptr）
         */
        template<class item_t>
        inline static void push(
            item_t ***              page_table_ptr, 
            uxx    *                length, 
            item_t const &          value, 
            inc::ialloc<void>       alloc, 
            inc::ifree<void>        free){

            auto & len                  = (length[0]);
            auto & tab                  = (page_table_ptr[0]);
            auto   need_new_page        = (len & (len - 1)) == 0; // 2^n
            auto   new_tab              = (item_t **)nullptr;
            auto   ptr                  = (item_t *)nullptr;
            auto   i_page               = (uxx)0;
            auto   i                    = (uxx)0;
            auto   mask                 = (uxx)initial_alloc_length - 1;
            auto   base                 = (inc::index_of_last_set(initial_alloc_length - 1)); // index_of_last_set(0) -> uxx(-1)

            if (len == 0) {
                if constexpr (not with_fixed_page_table){
                    tab                 = (item_t **)alloc(sizeof(voidp) * 2);
                }
                tab[0]                  = (item_t *)alloc(sizeof(item_t) * initial_alloc_length);
                tab[1]                  = (nullptr);
                ptr                     = (tab[0]);
                len                     = (1);
                xnew (ptr) item_t(value);
                return;
            }

            i                           = (inc::index_of_last_set(len | (initial_alloc_length - 1)));
            i_page                      = (i - base);
            mask                       |= (uxx(1) << i) - 1;

            if (need_new_page and len >= initial_alloc_length) {
                if (i_page % 2 == 0 and not with_fixed_page_table) { // i_page 是偶数页就需要分配新页表
                    new_tab             = (item_t **)alloc(sizeof(voidp) * (i_page + 2));
                    inc::copy(new_tab, tab, i_page);
                    free(tab, sizeof(voidp) * i_page);
                    tab                 = new_tab;
                    tab[i_page]         = nullptr;
                    tab[i_page + 1]     = nullptr;
                    ptr                 = tab[i_page];
                }

                if (tab[i_page] == nullptr) {
                    ptr                 = (item_t *)alloc(sizeof(item_t) * len);
                    tab[i_page]         = (ptr);
                }

                len                    += (1);
                xnew (ptr) item_t(value);
                return;
            }
            else{
                xnew (xref tab[i_page][len & mask]) item_t(value);
                len                    += (1);
            }
        }

        /* 函数：退栈
         * 参数：
         * - page_table_ptr 为指向页表数组的指针
         * - length 为数组长度指针
         * - value 为要获取退栈元素的指针
         * - alloc 为分配回调
         * - free 为回收回调
         */
        template<class item_t>
        inline static void pop(
            item_t ***              page_table_ptr, 
            uxx    *                length, 
            item_t *                value, 
            inc::ialloc<void>       alloc, 
            inc::ifree<void>        free){

            auto & len                  = (length[0]);
            auto & tab                  = (page_table_ptr[0]);
            auto & val                  = (value[0]);
            auto   new_tab              = (item_t **)nullptr;
            auto   i_page               = (uxx)0;
            auto   i                    = (uxx)0;
            auto   mask                 = (uxx)initial_alloc_length - 1;
            auto   base                 = (inc::index_of_last_set(initial_alloc_length - 1));
            auto   need_free_page       = (false);

            len                        -= (1);
            need_free_page              = (len & (len - 1)) == 0;
            i                           = (inc::index_of_last_set(len | (initial_alloc_length - 1)));
            i_page                      = (i - base);
            mask                       |= (uxx(1) << i) - 1;
            val                         = inc::move(tab[i_page][len & mask]); // 移动语义
            tab[i_page][len & mask].~item_t();

            if (not need_free_page) {
                return;
            }
            if (len >= initial_alloc_length * 2 and i_page % 2 == 0){
                if (tab[i_page + 1]){
                    free(tab[i_page + 1], sizeof(item_t) * len * 2);
                }

                free(tab[i_page], sizeof(item_t) * len);

                if constexpr (not with_fixed_page_table){
                    new_tab             = (item_t **)alloc(sizeof(voidp) * i_page);
                    inc::copy(new_tab, tab, i_page);
                    free(tab, sizeof(voidp) * (i_page + 2));
                    tab                 = new_tab;
                }
                return;
            }
            if (len == 0){
                free(tab[0], sizeof(item_t) * initial_alloc_length);
                free(tab[1], sizeof(item_t) * initial_alloc_length);

                if constexpr (not with_fixed_page_table){
                    free(tab, sizeof(voidp)  * 2);
                    tab                 = nullptr;
                }
            }
        }

        /* 函数：访问
         * 参数：
         * - page_table 为页表数组
         * - index 为要访问元素的索引
         */
        template<class item_t>
        inline static item_t & access(item_t ** page_table, uxx index){
            auto   i_page               = (uxx)0;
            auto   i                    = (uxx)0;
            auto   mask                 = (uxx)initial_alloc_length - 1;
            auto   base                 = (inc::index_of_last_set(initial_alloc_length - 1));

            i                           = (inc::index_of_last_set(index | (initial_alloc_length - 1)));
            i_page                      = (i - base);
            mask                       |= (uxx(1) << i) - 1;
            auto & val                  = (page_table[i_page][index & mask]);
            return val;
        }

        /* 函数：清空
         * 参数：
         * - page_table_ptr 为指向页表数组的指针
         * - length 为数组长度指针
         * - free 为回收回调
         */
        template<class item_t>
        inline static void clear(item_t *** page_table_ptr, uxx * length, inc::ifree<void> free){
            auto   len                  = inc::atom_swap<uxx>(xref length[0], 0);
            auto & tab                  = (page_table_ptr[0]);

            if (len == 0){
                return;
            }

            auto   i_page               = (uxx)0;
            auto   i                    = (uxx)0;
            auto   mask                 = (uxx)initial_alloc_length - 1;
            auto   current_length       = (uxx)initial_alloc_length;
            auto   multi                = (uxx)1;
            auto   base                 = (inc::index_of_last_set(initial_alloc_length - 1));

            len                        -= (1);
            i                           = (inc::index_of_last_set(len | (initial_alloc_length - 1)));
            mask                       |= (uxx(1) << i) - 1;
            i_page                      = (i - base);

            for(uxx i = 0; i < i_page; i++){
                for(uxx j = 0; j < current_length; j++){
                    tab[i][j].~item_t();
                }

                free(tab[i], sizeof(item_t) * current_length);
                tab[i]                  = nullptr;
                current_length          = initial_alloc_length * multi;
                multi                  *= 2;
            }

            for(uxx i = 0; i <= (len & mask); i++){
                tab[i_page][i].~item_t();
            }

            free(tab[i_page], sizeof(item_t) * current_length);
            tab[i_page]                 = nullptr;

            if (i_page % 2 == 0 and tab[i_page += 1] != nullptr){
                if (i_page > 1){
                    current_length     *= 2;
                }
                free(tab[i_page], sizeof(item_t) * current_length);
                tab[i_page]             = nullptr;
            }

            if constexpr (not with_fixed_page_table){
                free(tab, sizeof(voidp) * (i_page + 1));
                tab                     = nullptr;
            }
        }
    };
}

#endif

xexport_space(mixc::algo_mmu::origin)
