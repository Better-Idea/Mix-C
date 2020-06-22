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
 *     - 通过所属进程 id + 线程 id 保护 + 共享内存
 *     - 包含小页(对象组专用)的高 24 位作为对象组内成员的公共段地址
 * - 包含 252 * 4Byte 对象
 * - 当与之绑定的小页内存分配完毕时，该对象组将不能再分配对象
 * - 只有当小页不能分配所需大小的内存时，才能为该对象组所属的线程分配新的对象组
 * 
 * 对象组专用小页：
 * - 大小 64KB
 * - 满足 数据分区 中的小页说明
 * - 可以被多个对象组绑定，但这些对象组必须属于同一个线程
 * - 
 * 
 */ 
#ifndef xpack_algo_mmu
#define xpack_algo_mmu
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_mmu
        #include"define/base_type.hpp"
        #include"instruction/index_of_last_set.hpp"
        #include"instruction/bit_test_and_reset.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")


    namespace mixc::algo_mmu {
        struct pagoff{ uxx page, offset; };

        struct addressing{
            static auto var(uxx present_indicator, uxx address){
                uxx    mask = present_indicator & address;
                pagoff r { .page = inc::index_of_last_set(mask) + 1, .offset = address };

                if (r.page != 0){
                    inc::bit_test_and_reset(& r.offset, r.page);
                }
                return r;
            }

            static auto fixed(uxx present_indicator, uxx address){
                pagoff r;
                uxx mask = present_indicator ^ address;
                r.page   = inc::index_of_last_set(mask);
                r.offset = address;
                return r;
            }
        };

        template<uxx small_page_count, uxx step_exp = 8>
        union page_var{
            using the_t = page_var<small_page_count, step_exp>;
        //     static auto alloc(uxx capacity){
        //         return inc::alloc_with_initial<the_t>(
        //             inc::memory_size(
        //                 size(capacity)
        //             ),
        //             capacity
        //         );
        //     }
        // protected:
        //     page_var(uxx capcity) : 
        //         capcity(capcity), bytes(0){}

            static uxx size(uxx capacity){
                uxx     i     = (inc::index_of_last_set(capacity) + 1);
                uxx     mask  = (uxx(1) << small_page_count) - 1;
                uxx     spc   = (i >> (step_exp)) & mask;
                uxx     bpc   = (i >> (step_exp + small_page_count));
                uxx     bytes = (bpc + spc + 2) * sizeof(uxx);
                return  bytes;
            }

            template<class type>
            type & access(uxx address){
                using typep = type *;
                auto r = addressing::var(bytes, address);

                if (r.page < step_exp){
                    return typep(spage[0] + r.offset)[0];
                }
                if (r.page -= step_exp; r.page < small_page_count){
                    return typep(spage[r.page] + r.offset)[0];
                }

                r.page    -= small_page_count;
                r.offset  &= bpage_mask;
                return typep(bpage[r.page] + r.offset)[0];
            }

            uxx    bytes;
            uxx    capcity;
            u08 *  spage[small_page_count];
            u08 *  bpage[1];

            static constexpr uxx bpage_size = uxx(1) << (step_exp + small_page_count);
            static constexpr uxx bpage_mask = bpage_size - 1;

            #define xgen(name,type)                                                 \
            struct{                                                                 \
                type & operator [](uxx address){                                    \
                    return the.template access<type>(address * sizeof(type));       \
                }                                                                   \
            } name
        public:
            xgen(mu08, u08);
            xgen(mu16, u16);
            xgen(mu32, u32);
            xgen(mu64, u64);
            xgen(mi08, i08);
            xgen(mi16, i16);
            xgen(mi32, i32);
            xgen(mi64, i64);
            xgen(mf32, f32);
            xgen(mf64, f64);
            #undef xgen
        };

    }

#endif

namespace xuser::inc{
    using ::mixc::algo_mmu::addressing;
    using ::mixc::algo_mmu::page_var;
}
