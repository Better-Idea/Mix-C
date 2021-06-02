#ifndef xpack_extern_rtos_memory
#define xpack_extern_rtos_memory 
#pragma push_macro("xuser")  
#undef  xuser
#define xuser mixc::extern_rtos_memory::inc
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"instruction/bit_test.hpp"
#include"instruction/bit_test_and_set.hpp"
#include"instruction/bit_test_and_reset.hpp"
#include"instruction/index_of_first_set.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"macro/xref.hpp"
#include"memop/zeros.hpp"
#include"meta/has_constructor.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_rtos_memory{
    enum : uxx{
        shift_to_get_scale_one  = 6,
        shift_to_next_level     = shift_to_get_scale_one,
        scale_one               = 1 << shift_to_get_scale_one,
        scale_two               = scale_one * scale_one,
        scale_level             = 2,

        // 小块以 size_one 为区分单位
        // 无论是大块还是小块，size_one 都是分配的最小单位
        size_one                = 16,
        size_two                = size_one * scale_one,
        size_three              = size_two * scale_one,
        mask_to_get_header      = (1 << 16) - 1,
        max_slot                = scale_one,
    };

    struct page_header;

    typedef struct node{
        node      * prev;
        node      * next;

        page_header * offset_to_header(){
            auto begin = uxx(this) & ~mask_to_get_header;
            return (page_header *)begin;
        }

        #if xis_os32
    private:
        uxx         paddingx[2];
        #endif
    } * nodep;

    typedef struct node_plus : node{
        // 当前块长度（单位为 size_one）是多少
        uxx         blocks;

        // 该参数用于对齐
        uxx         padding;

        #if xis_os32
    private:
        uxx         paddingx[2];
        #endif
    } * node_plusp;

    typedef struct page_header{
    private:
        nodep         block_ptr;
        u16           bmp_bytes;
        u16           blocks;
        struct pair{ nodep begin = nullptr; uxx length = 0; };

        u08p bmp_area(){
            return u08p(this + 1);
        }

        nodep data_area(){
            return nodep(block_ptr);
        }

    public:
        page_header *  prev;
        page_header *  next;
        page_header(uxx bytes){
            bmp_bytes = bytes / size_one / 8;
            auto cost = sizeof(page_header) + bmp_bytes;
            cost     += (size_one - 1);
            cost     &= (~(size_one - 1));
            blocks    = (bytes - cost) / size_one;
            block_ptr = (nodep(uxx(this) + bytes)) - blocks;
            inc::zeros(this->bmp_area(), bmp_bytes);

            // 设置哨兵位
            set(uxx(-1));
            set(blocks);
        }

        uxx total_blocks(){
            return blocks;
        }

        bool get(uxx index){
            index += 1;
            return inc::bit_test(bmp_area()[index >> 3], index & 0x7);
        }

        // 标记占用
        void set(uxx index){
            index += 1;
            inc::bit_test_and_set(xref(bmp_area()[index >> 3]), index & 0x7);
        }

        // 标记空闲
        void reset(uxx index){
            index += 1;
            inc::bit_test_and_reset(xref(bmp_area()[index >> 3]), index & 0x7);
        }

        uxx index_of(nodep node){
            return node - this->data_area();
        }

        pair left_free_block_of(nodep current){
            if (uxx index = index_of(current); get(index - 1)){
                return pair{};
            }
            else if (get(index - 2)){
                return pair{ current - 1, 1 };
            }
            else{
                auto plus = node_plusp(current) - 1;
                auto blocks = plus->blocks;
                return pair{ current - blocks, blocks };
            }
        }

        pair right_free_block_of(nodep current, uxx length){
            if (uxx index = index_of(current += length); get(index)){
                return {};
            }
            else if (get(index + 1)){
                return { current, 1 };
            }
            else{
                return { current, node_plusp(current)->blocks };
            }
        }

        nodep first_block(){
            return block_ptr;
        }

        void set_rest(nodep current, uxx length){
            if (length <= 1){
                return;
            }

            auto end        = node_plusp(nodep(current + length)) - 1;
            auto head       = node_plusp(current);
            head->blocks    = length;
            end->blocks     = length;
        }

        void mark_in_use(nodep current, uxx length){
            uxx index = index_of(current);
            set(index);
            set(index + length - 1);
        }

        void mark_free(nodep current, uxx length){
            uxx index = index_of(current);
            reset(index);
            reset(index + length - 1);
        }
    } * page_headerp;

    struct area_t{
        uxx index_of_closest(uxx i_slot){
            u64 mask = ~((u64(1) << i_slot) - 1);
            u64 masked = area & mask;
            return inc::index_of_first_set(masked);
        }

        bool test_and_reset(uxx index){
            return inc::bit_test_and_reset(xref(area), index);
        }

        bool test_and_set(uxx index){
            return inc::bit_test_and_set(xref(area), index);
        }
    private:
        u64 area = 0;
    };

    struct memory_header{
    protected:
        area_t          area[scale_level]       = {};
        uxx             pused_bytes             = 0;
        uxx             palive_object           = 0;
        page_header   * page_list               = nullptr;
        node          * free_list_array[scale_level][max_slot];
    };
}

namespace mixc::extern_rtos_memory::origin{
    struct memory : memory_header{
        /**
         * @brief 获取当前内存已经使用的字节数
         */
        uxx used_bytes() const {
            return pused_bytes;
        }

        /**
         * @brief 获取当前内存存活的对象数（待释放的内存个数）
         */
        uxx alive_object() const {
            return palive_object;
        }

        /**
         * @brief 带构造的内存分配
         * @param list 当前对象的构造参数
         * @return 可用的内存，失败时返回 nullptr
         */
        template<class item_t, class ... args_t>
        requires(inc::has_constructor<item_t, void(args_t...)>)
        item_t * alloc(args_t const & ... list){
            auto ptr = this->alloc(sizeof(item_t));
            if (ptr == nullptr){
                return nullptr;
            }
            return xnew(ptr) item_t(list...);
        }

        /**
         * @brief 内存分配
         * @param bytes 需要分配内存的字节数
         * @return 可用的内存，失败时返回 nullptr
         */
        voidp alloc(uxx bytes){
            auto i_expect           = (bytes - 1) / size_one;
            auto i_slot             = (i_expect);
            auto expect_blocks      = (i_expect + 1);

            // 目的：
            // area 中置位位表示空闲的块
            // 尽可能选择最接近但不小于所需大小的块
            // 详情：
            // 每个分区有 64(max_slot) 个槽，一共有 scale_level 个分区
            // 0 级分区的步进是 16Byte(size_one)
            // 1 级分区的步进是 1024Byte(size_two)
            // 假如需要 1400Byte 内存
            // 0 级分区中是一定没有匹配项的，因为它只负责 1～1024Byte 内存
            // 所以需要到 1 级分区中寻找，该分区的 0 号槽负责的是 1025~2048Byte 的内存块
            // 但该槽不一定能提供满足所需大小的块，所以需要到下一个分区开始寻找
            // 1400Byte 对应的 i = 1、i_expect = 87、i_slot = 1
            for(uxx i = 0; i < scale_level; i++, i_slot >>= shift_to_next_level){
                if (i_slot >= max_slot){
                    ;
                }
                else if (auto i_closest = area[i].index_of_closest(i_slot); i_closest != not_exist){
                    pused_bytes    += (bytes);
                    palive_object  += (1);
                    return this->take_out(i_closest, expect_blocks, area[i], free_list_array[i]);
                }
            }
            return nullptr;
        }

        /**
         * @brief 待析构的内存释放
         * @param ptr 当前对象
         */
        template<class item_t>
        void free(item_t const * ptr){
            if (ptr == nullptr){
                return;
            }

            ptr->~item_t();
            this->free(ptr, sizeof(item_t));
        }

        /**
         * @brief 带析构的内存释放
         * @param block 待释放的内存
         * @param bytes 待释放内存的大小
         */
        void free(void const * block, uxx bytes){
            if (block == nullptr or bytes == 0){
                return;
            }

            auto ptr                = (nodep)block;
            auto page               = (ptr->offset_to_header());
            auto i_expect           = (bytes - 1) / size_one;
            auto free_blocks        = (i_expect + 1);
            auto left               = (page->left_free_block_of(ptr));
            auto right              = (page->right_free_block_of(ptr, free_blocks));
            pused_bytes            -= (bytes);
            palive_object          -= (1);

            if (page->mark_free(ptr, free_blocks); left.begin != nullptr){
                page->mark_free(left.begin, left.length);
                ptr                 = left.begin;
                free_blocks        += left.length;
                this->remove(left.begin, left.length);
            }
            if (right.begin != nullptr){
                free_blocks        += right.length;
                page->mark_free(right.begin, right.length);
                this->remove(right.begin, right.length);
            }

            // 合成的更大的空闲块放回对应的链表
            this->put_back(page, ptr, free_blocks);
        }

        /**
         * @brief 挂载内存页
         * @param ptr 对齐的内存页地址
         * @param bytes 页的大小
         * @return 挂载成功与否
         */
        bstate_t mount(voidp ptr, uxx bytes){
            // 地址没有对齐
            if (uxx(ptr) & mask_to_get_header){
                return bstate_t::fail;
            }

            auto page = xnew(ptr) page_header(bytes); 

            // 如果是首个挂载的内存页，则作为页列表的首元
            if (page_list == nullptr){
                page_list           = page;
                page->next          = page->prev = page;
            }
            // 否则追加到元素末尾
            else{
                auto prev           = page_list->prev;
                prev->next          = page;
                page->prev          = prev;
                page_list->prev     = page;
                page->next          = page_list;
            }

            // 将页添加到对应的槽中
            this->put_back(page, page->first_block(), page->total_blocks());
            return bstate_t::success;
        }

        void process_message() {

        }

    private:
        /**
         * @brief 将合并或剩余的内存块存回对应的 area 中
         * @param page 当前 ptr 内存块所在的页
         * @param ptr 合并或剩余的内存块
         * @param free_blocks 块长度（单位为 size_one）
         */
        void put_back(page_header * page, nodep ptr, uxx free_blocks){
            for(uxx i = 0, i_slot = free_blocks - 1; i < scale_level; i++){
                if (i_slot < max_slot){
                    this->append(ptr, area[i], i_slot, free_list_array[i]);
                    break;
                }
                if (i_slot){
                    i_slot          = (i_slot - 1) >> shift_to_next_level;
                }
            }

            page->set_rest(ptr, free_blocks); 
        }

        /**
         * @brief 将所需大小的内存块取出来并将剩余的内存块放回
         * @param i_closest 最接近所需块大小的槽（索引，最大值为 64（max_slot））
         * @param expect_blocks 所需的块长度（单位为 size_one）
         * @param area 可用块所在的分区
         * @param free_list 可用块所在的空闲链表
         * @return 可用内存
         */
        voidp take_out(uxx i_closest, uxx expect_blocks, area_t & area, node ** free_list){
            auto first              = free_list[i_closest];
            auto page               = first->offset_to_header();
            auto next               = first->next;

            // 块长度大于 1 的块存放了 blocks 字段
            // 块长度等于 1 的块可能没有多余的空间存放 blocks 字段
            auto real_blocks        = expect_blocks == 1 ? 1 : node_plusp(first)->blocks;

            // 若当前块时该链表中最后一个元素，则将该位置的 area 设置为空
            if (next == first){
                area.test_and_reset(i_closest);
            }
            // 否则让下一个元素顶替 first 的位置
            else{
                next->prev          = first->prev;
                first->prev->next   = next;
                free_list[i_closest]= next;
            }

            // first 作为分配出去的块，rest 作为剩下的块放回
            if (page->mark_in_use(first, expect_blocks); real_blocks > expect_blocks){
                auto rest           = xref(first[expect_blocks]);
                auto rest_blocks    = real_blocks - expect_blocks;
                this->put_back(page, rest, rest_blocks);
            }
            return first;
        }

        /**
         * @brief 将可用块追加到对应的链表槽中
         * @param ptr 存在于槽中的可用内存块
         * @param area 槽位图
         * @param i_slot 内存块 ptr 所在的槽下标
         * @param free_list 内存块 ptr 将要存放的链表
         */
        void append(node * ptr, area_t & area, uxx i_slot, node ** free_list){
            // 若当前槽不是空的，就将 ptr 追加到链表末尾
            if (area.test_and_set(i_slot)){
                auto header         = free_list[i_slot];
                ptr->prev           = header->prev;
                ptr->next           = header;
                header->prev->next  = ptr;
                header->prev        = ptr;
            }
            // test_and_set 将当前槽设置为非空
            // 当前 ptr 作为该链表槽的首元
            else{
                free_list[i_slot]   = ptr;
                ptr->next           = ptr;
                ptr->prev           = ptr;
            }
        }

        /**
         * @brief 将可用块从槽中移除
         * @param ptr 存在于槽中的可用内存块
         * @param area 槽位图
         * @param i_slot 内存块 ptr 所在的槽下标
         * @param free_list 内存块 ptr 所在的链表
         */
        void remove(node * ptr, area_t & area, uxx i_slot, node ** free_list){
            auto next               = ptr->next;
            auto prev               = ptr->prev;

            // 如果当前元素是最后一个元素，就复位 i_slot 对应的指示位
            if (next == ptr){
                area.test_and_reset(i_slot);
                return;
            }

            // 不再判断 ptr 如果是首元的情况了，这里直接让 next 变成首元
            free_list[i_slot]       = next;
            next->prev              = prev;
            prev->next              = next;
        }

        /**
         * @brief 将可用块从槽中移除
         * @param ptr 存在于槽中的可用内存块
         * @param blocks 块长度（单位为 size_one）
         */
        void remove(node * ptr, uxx blocks){
            for(uxx i = 0, i_slot = blocks - 1; i < scale_level; i++){
                if (i_slot < max_slot){
                    this->remove(ptr, area[i], i_slot, free_list_array[i]);
                    break;
                }

                // remove 和 take_out 不同
                // 位于 0 分区（不含）以上的分区，即使需要分配的内存大小落在对应的槽中
                // 也是不能在该槽中获取可用块的，对于请求 1400Byte 的内存
                // 1 分区 0 号槽管理的内存就包含这个范围的内存，但它也包含小于这个请求大小的内存
                // 而槽内部是不再区分块大小的，所以这个分区只适合给小于它管理范畴的请求分配内存
                // 但是 remove 时就无需考虑这一点，因为要移除的块就在这个链表槽里
                // 以 blocks = 65 为例，此时 i = 0, i_slot = 64，由于 i_slot >= max_slot
                // 所以需要进入下一个分区，i = 1, i_slot = (64 - 1) >> 6 = 0
                // 也就是 1 分区 0 号槽满足要求
                if (i_slot){
                    i_slot          = (i_slot - 1) >> shift_to_next_level;
                }
            }
        }
    };
}

#endif

xexport_space(mixc::extern_rtos_memory::origin)
