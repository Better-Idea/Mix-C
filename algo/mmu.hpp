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
#define xuser mixc::algo_mmu
#include"instruction/bit_test_and_reset.hpp"
#include"instruction/index_of_last_set.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_free.hpp"
#include"memop/copy.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_mmu {
    struct pagoff{ uxx page, offset; };

    struct addressing{
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

        static pagoff fixed(uxx present_indicator, uxx address){
            pagoff r;
            uxx mask = present_indicator ^ address;
            r.page   = inc::index_of_last_set(mask);
            r.offset = address;
            return r;
        }
    };

    template<uxx begin_capacity = 1>
    struct var_array {
        static_assert((begin_capacity & (begin_capacity - 1)) == 0 and begin_capacity > 0);

        template<class item_t>
        inline static void push(item_t *** page_table_ptr, uxx * length, item_t const & value, inc::can_alloc<void> alloc, inc::can_free<void> free){
            auto & len                  = (length[0]);
            auto & tab                  = (page_table_ptr[0]);
            auto   need_new_page        = (len & (len - 1)) == 0;
            auto   new_tab              = (item_t **)nullptr;
            auto   ptr                  = (item_t *)nullptr;
            auto   i                    = (uxx)0;
            auto   ii                   = (uxx)0;
            auto   mask                 = (uxx)0;
            auto   base                 = (inc::index_of_last_set(begin_capacity - 1));

            if (len == 0) {
                tab                     = (item_t **)alloc(sizeof(voidp) * 2);
                tab[0]                  = (item_t *)alloc(sizeof(item_t) * begin_capacity);
                tab[1]                  = (nullptr);
                ptr                     = (tab[0]);
                len                     = (1);
                new (ptr) item_t(value);
                return;
            }

            ii                          = (inc::index_of_last_set(len | (begin_capacity - 1)));
            mask                        = (uxx(1) << ii) - 1;
            i                           = (ii - base);

            if (need_new_page and len >= begin_capacity) {
                if ((i & 1) == 0) { // i 是偶数页就需要分配新页表
                    new_tab             = (item_t **)alloc(sizeof(voidp) * (i + 2));
                    inc::copy(new_tab, tab, i);
                    free(tab, sizeof(voidp) * i);
                    tab                 = new_tab;
                    tab[i]              = nullptr;
                    tab[i + 1]          = nullptr;
                    ptr                 = tab[i];
                }

                if (tab[i] == nullptr) {
                    ptr                 = (item_t *)alloc(sizeof(item_t) * len);
                    tab[i]              = (ptr);
                }

                len                    += (1);
                new (ptr) item_t(value);
                return;
            }
            else{
                new (xref tab[i][len & mask]) item_t(value);
                len                    += 1;
            }
        }

        template<class item_t>
        inline static void pop(item_t *** page_table_ptr, uxx * length, item_t * value, inc::can_alloc<void> alloc, inc::can_free<void> free){
            auto & len                  = (length[0]);
            auto & tab                  = (page_table_ptr[0]);
            auto & val                  = (value[0]);
            auto   new_tab              = (item_t **)nullptr;
            auto   i                    = (uxx)0;
            auto   ii                   = (uxx)0;
            auto   mask                 = (uxx)0;
            auto   base                 = (inc::index_of_last_set(begin_capacity - 1));
            auto   need_free_page       = (false);

            len                        -= (1);
            need_free_page              = (len & (len - 1)) == 0;
            ii                          = (inc::index_of_last_set(len | (begin_capacity - 1)));
            mask                        = (uxx(1) << ii) - 1;
            i                           = (ii - base);
            val                         = (tab[i][len & mask]);
            tab[i][len & mask].~item_t();

            if (not need_free_page) {
                return;
            }
            if (len >= begin_capacity * 2 and (i & 1) == 0){
                if (tab[i + 1]){
                    free(tab[i + 1], sizeof(item_t) * len * 2);
                }
                free(tab[i]    , sizeof(item_t) * len);
                new_tab = (item_t **)alloc(sizeof(voidp) * i);
                inc::copy(new_tab, tab, i);
                free(tab       , sizeof(voidp) * (i + 2));
                tab                     = new_tab;
                return;
            }
            if (len == 0){
                free(tab[0]    , sizeof(item_t) * begin_capacity);
                free(tab[1]    , sizeof(item_t) * begin_capacity);
                free(tab       , sizeof(voidp) * (2));
                tab                     = nullptr;
            }
        }

        template<class item_t>
        inline static item_t & access(item_t ** page_table, uxx length){
            auto   len                  = (length);
            auto   i                    = (uxx)0;
            auto   ii                   = (uxx)0;
            auto   mask                 = (uxx)0;
            auto   base                 = (inc::index_of_last_set(begin_capacity - 1));

            len                        -= (1);
            ii                          = (inc::index_of_last_set(len | (begin_capacity - 1)));
            mask                        = (uxx(1) << ii) - 1;
            i                           = (ii - base);
            auto & val                  = (page_table[i][len & mask]);
            return val;
        }
    };
}

#endif

xexport(mixc::algo_mmu::pagoff)
xexport(mixc::algo_mmu::addressing)
xexport(mixc::algo_mmu::var_array)
