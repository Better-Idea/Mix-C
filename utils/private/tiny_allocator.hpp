#ifndef xpack_utils_private_tiny_allocator
#define xpack_utils_private_tiny_allocator 
#pragma push_macro("xuser")  
#undef  xuser
#define xuser mixc::utils_private_tiny_allocator::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/lock/mutex.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"define/base_type.hpp"
#include"instruction/bit_test.hpp"
#include"instruction/bit_test_and_set.hpp"
#include"instruction/bit_test_and_reset.hpp"
#include"macro/xexport.hpp"
#include"macro/xdebug+.hpp"
#include"macro/xdefer.hpp"
#include"macro/xnew.hpp"
#include"utils/allocator.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_private_tiny_allocator::origin{
    struct tiny_allocator;
}

namespace mixc::utils_private_tiny_allocator{
    typedef struct node{
        node      * previous;
        node      * next;
    } * nodep;

    typedef struct node_free{
        node_free * next;
        uxx         bytes;
    } * node_freep;

    typedef struct node_plus : node{
        uxx    blocks;
    } * node_plusp;

    enum : uxx{
        scale                   = sizeof(uxx) * 8,

        // 小块以 scale_one 为区分单位
        // 无论是大块还是小块，scale_one 都是分配的最小单位
        scale_one               = sizeof(node),

        // 大块以 scale_two 为区分单位
        scale_two               = scale_one * scale,

        // 块数不超过该值定义为小块，否为定义为大块
        boundary                = scale,

        // 每次从底层分配器中获取一个页来管理，每一个页都是按 page_bytes 对齐
        page_bytes              = scale_two * scale,

        // 页对齐大小
        page_align_size         = page_bytes,

        // 用于获取页内偏移
        mask_to_get_offset      = page_bytes - 1,

        // 用于获取页首地址
        mask_to_get_header      = ~mask_to_get_offset,

        // 页首部位指示器位数
        max_bits                = page_bytes / scale_one,

        // 跳过检查的次数
        // 从其他线程推送过来的释放请求
        max_skip_times          = 8,
    };

    typedef struct page_header{
    private:
        u08                         bmp[max_bits / 8] = {0};
        struct pair{ nodep begin = nullptr; uxx length = 0; };
    public:
        page_header(origin::tiny_allocator * owner);

        bool get(uxx index){
            index += 1;
            return inc::bit_test(bmp[index >> 3], index & 0x7);
        }

        // 标记占用
        void set(uxx index){
            index += 1;
            inc::bit_test_and_set(xref bmp[index >> 3], index & 0x7);
        }

        // 标记空闲
        void reset(uxx index){
            index += 1;
            inc::bit_test_and_reset(xref bmp[index >> 3], index & 0x7);
        }

        uxx index_of(nodep node){
            return node - nodep(this + 1);
        }

        pair left_free_block_of(nodep current){
            if (uxx index = index_of(current); get(index - 1)){
                return pair{};
            }
            else if (get(index - 2)){
                return pair{ current - 1, 1 };
            }
            else{
                auto len = *uxxp(current - 1);
                return pair{ current - len, len };
            }
        }

        pair right_free_block_of(nodep current, uxx length){
            if ((mask_to_get_offset & uxx(current += length)) == 0){
                return {};
            }
            if (uxx index = index_of(current); get(index)){
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
            return nodep(this + 1);
        }

        void set_rest(nodep current, uxx length){
            if (length <= 1){
                return;
            }
            (node_plusp(current))->blocks   = length;
            (uxxp(current + length - 1))[0] = length;
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

        page_header               * previous;
        page_header               * next;
        origin::tiny_allocator    * owner;
        voidp                     * reserved; // 对齐
    } * page_headerp;

    enum : uxx{
        // 一个页一共有多少可用块
        blocks_per_page         = (page_bytes - sizeof(page_header)) / scale_one,
    };

    inline page_header::page_header(origin::tiny_allocator * owner):
        owner(owner){

        // 设置哨兵位
        set(uxx(-1));
        set(blocks_per_page);
    }

    // 必须是 2 的次幂
    static_assert((page_bytes & (page_bytes - 1)) == 0);
}

namespace mixc::utils_private_tiny_allocator::origin{
    struct tiny_allocator_header{
        using indicator_t = inc::bits_indicator<scale>;

        indicator_t     slot;
        indicator_t     slot_plus;
        uxx             async_push_bytes        = 0;
        uxx             palive_pages            = 0;
        uxx             pused_bytes             = 0;
        uxx             pneed_free_count        = 0;
        uxx             skip_count              = 0;
        page_header *   page_list               = nullptr;
        node_free   *   async_pend_head         = nullptr;
        node_free   *   async_pend_tail         = nullptr;
        node_free   *   async_free_list_head    = nullptr;
        node_free   *   async_free_list_tail    = nullptr;
        node        *   free_list_array[scale];
        node        *   free_list_array_plus[scale];
    };

    struct tiny_allocator;
    inline uxx                      remainder_alive_pages;
    inline uxx                      remainder_used_bytes;
    inline uxx                      remainder_need_free_count;
    inline inc::mutex               once;
    inline u08                      mirror_allocator[sizeof(tiny_allocator_header)];
    inline tiny_allocator *         recycler = (tiny_allocator *)xnew(mirror_allocator)tiny_allocator_header();

    struct tiny_allocator : tiny_allocator_header{
        static void do_clean(){
            auto new_alive_pages            = uxx(0);
            auto new_used_bytes             = uxx(0);
            auto new_need_free_count        = uxx(0);

            while(true){
                // 后续补充信号量作为等待对象，这里暂时使用固定的等待10ms
                inc::thread_self::sleep(10);

                // 先获取 remainder_used_bytes
                // 与 ~tiny_allocator 保持协调
                if (new_used_bytes = inc::atom_swap<uxx>(xref remainder_used_bytes, 0);
                    new_used_bytes == 0 and recycler->pused_bytes == 0){
                    continue;
                }

                new_alive_pages             = inc::atom_swap<uxx>(xref remainder_alive_pages, 0);
                new_need_free_count         = inc::atom_swap<uxx>(xref remainder_need_free_count, 0);
                recycler->palive_pages     += new_alive_pages;
                recycler->pused_bytes      += new_used_bytes;
                recycler->pneed_free_count += new_need_free_count;

                // 先处理计数器，后处理释放操作
                recycler->async_pop();
            }
        }

        ~tiny_allocator(){
            if (need_free_count() == 0){
                return;
            }

            auto head                       = (page_list);
            auto current                    = (page_list);
            auto aysnc_free_size            = (inc::atom_load(xref async_push_bytes));
            auto all_return                 = (aysnc_free_size == need_free_count());

            if (all_return){
                this->async_pop();
                return;
            }

            // windows 平台：
            // 在 tiny_allocator::~tiny_allocator 创建内存清理守护线程的父线程没有退出 thread_local 析构的代码段时
            // 守护线程将无法得到执行，所以这里静态分配了 tiny_allocator 对象 recycler
            // 当需要时再创建清理线程
            // 如果其他线程已经归还所有属于本线程的内存就无需
            if (once.try_lock() == inc::lock_state_t::accept){
                inc::thread(xdetached{
                    tiny_allocator::do_clean();
                });
            }

            do{
                inc::atom_store(xref current->owner, recycler);
                current                     = current->next;
            }while(current != head);

            // 再做一次清理，在转交所有权前可能其他线程推送释放内容
            // 内部会再计算 palive_pages、pused_bytes、pneed_free_count 这些字段
            this->async_pop();

            // 等 async_pop 处理完成后再计算
            inc::atom_add(xref remainder_alive_pages, palive_pages);
            inc::atom_add(xref remainder_need_free_count, pneed_free_count);

            // 最后设置，确保读到该值时之前的 remainder_alive_pages 和 remainder_need_free_count 都已有效
            inc::atom_add(xref remainder_used_bytes, pused_bytes);
        }

        uxx used_bytes() {
            return pused_bytes;
        }

        uxx need_free_count() {
            return pneed_free_count;
        }

        uxx alive_pages(){
            return palive_pages;
        }

        voidp alloc(uxx bytes){
            auto require_size_index = (bytes - 1) / scale_one;
            pused_bytes            += (bytes);
            pneed_free_count       += (1);

            // slot 中置位位表示空闲的块
            // 选择最接近但不小于所需大小的块
            if (require_size_index < boundary){
                if (auto closest_index = slot.index_of_first_set(require_size_index); closest_index != not_exist){
                    return take_out(closest_index, require_size_index, slot, free_list_array);
                }
            }
            if (require_size_index < blocks_per_page){
                if (auto closest_index = slot_plus.index_of_first_set(require_size_index / boundary); closest_index != not_exist){
                    return take_out(closest_index, require_size_index, slot_plus, free_list_array_plus);
                }
            }
            return origin_alloc(require_size_index);
        }

        void free(voidp ptr, uxx bytes){
            xdebug_fail(bytes == 0){
                return;
            }

            auto   scale_index      = (bytes - 1) / scale_one; 
            auto   current          = (nodep)ptr;
            auto & head             = (get_page_header_by(current));
            auto   owner            = (head.owner);

            // 超出管理的大小
            if (scale_index >= blocks_per_page){
                palive_pages       -= 1;
                pneed_free_count   -= 1;
                pused_bytes        -= bytes;
                inc::mfree(ptr);
                return;
            }

            // 处理其他线程归还的内存
            if (owner == this){
                this->async_pop_lazily();
                this->free_core(ptr, bytes);
                return;
            }
            
            // 分配该内存的线程可能已经退出并将所有权转交给后台内存清理线程
            // cpu 读了一次 head.owner 后可能会被缓存下来
            // 所以该改动需要原子操作才能读取到
            owner                   = inc::atom_load(xref head.owner);

            // 如果该内存属于其他线程，就推送到分配它的线程
            owner->async_push(current, bytes);
        }

    private:
        void free_core(voidp ptr, uxx bytes){
            auto   current          = (nodep)ptr;
            auto & head             = (get_page_header_by(current));
            auto   scale_index      = (bytes - 1) / scale_one;
            auto   free_block       = (current);
            auto   free_size        = (scale_index + 1);
            auto   left             = (head.left_free_block_of(current));
            auto   right            = (head.right_free_block_of(current, free_size));
            pused_bytes            -= (bytes);
            pneed_free_count       -= (1);

            if (head.mark_free(current, scale_index + 1); left.begin != nullptr){
                head.mark_free(left.begin, left.length);
                free_block          = left.begin;
                free_size          += left.length;
                remove(left.begin, left.length);
            }
            if (right.begin != nullptr){
                free_size          += right.length;
                head.mark_free(right.begin, right.length);
                remove(right.begin, right.length);
            }

            // 整个页都空闲时就释放该页
            if (free_size == blocks_per_page){
                origin_free(xref head);
                return;
            }

            // 否则合成的更大的空闲块放回对应的链表数组中
            append(free_block, free_size);
        }

        void async_push(voidp mem, uxx bytes){
            auto self               = node_freep(mem);
            auto prev               = node_freep(nullptr);
            self->next              = nullptr;
            self->bytes             = bytes;
            prev                    = inc::atom_swap(xref async_free_list_tail, self);
            inc::atom_add(xref async_push_bytes, bytes);

            if (prev == nullptr){
                inc::atom_store(xref async_free_list_head, self);
            }
            else{
                inc::atom_store(xref prev->next, self);
            }
        }

        void async_pop_lazily(){
            // 忽略一定次数后就响应一下
            // 尽可能减少无效的原子操作
            if (skip_count += 1; skip_count == max_skip_times){
                skip_count          = 0;
                async_pop();
            }
        }

        void async_pop(){
            auto next               = node_freep(nullptr);
            auto null               = node_freep(nullptr);
            auto head               = node_freep(nullptr);
            auto free_size          = 0;

            xdefer{
                inc::atom_sub(xref async_push_bytes, free_size);
            };

            // 只要出现了当前要释放节点的下一个节点为 nullptr，但它又不是最后一个节点
            // 此时属于链接未完成的情况，我们可以先不管它
            while(async_pend_head != async_pend_tail){
                // 可能其他 cpu 缓存信息还未同步到当前 cpu
                // 所以需要在读到 nullptr 的 next 时还需要通过原子操作再读一次
                // 这里事先不用原子操作读取是为了避免性能损失
                if (next = async_pend_head->next; next == nullptr){
                    if (next = inc::atom_load(xref async_pend_head->next);
                        next == nullptr){
                        return;
                    }
                }

                free_size          += async_pend_head->bytes;
                free_core(async_pend_head, async_pend_head->bytes);
                async_pend_head     = next;
            }

            // 拿走首节点
            if (head = inc::atom_swap(xref async_free_list_head, null); 
                head == nullptr){
                return;
            }

            while(true){
                // 如果存在下一个节点就释放当前节点
                if (next = head->next; next != nullptr){
                    free_size      += head->bytes;
                    free_core(head, head->bytes);
                    head            = next;
                    continue;
                }

                // 竞争尾节点
                // 如果其他线程没有获取到 async_free_list_tail
                // 那么就不会设置 async_free_list_tail->next
                // 因为 async_free_list_tail is head 
                // 所以我们可以安全的释放掉该节点
                if (next = inc::atom_swap(xref async_free_list_tail, null); 
                    next == head){
                    free_size      += head->bytes;
                    free_core(head, head->bytes);
                    return;
                }
                // 此时处于其他线程已经获得了 async_free_list_tail
                // 但还未修改 async_free_list_tail->next 的状态
                // 此时将产生待定的链表（未完成链接），需要下一次进入该函数时先处理它
                // 由于我们把 async_free_list_tail 置 null
                // 导致其他线程认为现在是空节点的状态
                // 第一个设置 async_free_list_tail 的其他线程会再设置 async_free_list_head
                // 此时我们将有两组待释放的链表
                else{
                    async_pend_head = head;
                    async_pend_tail = next;
                    return;
                }
            }
        }

        void origin_free(page_header * ptr){
            auto next      = ptr->next;
            auto prev      = ptr->previous;
            inc::mfree_aligned(ptr);
            palive_pages  -= 1;

            if (next == ptr){
                page_list  = nullptr;
                return;
            }
            if (ptr == page_list){
                page_list  = next;
            }

            next->previous = prev;
            prev->next     = next;
        }

        voidp origin_alloc(uxx require_size_index){
            // 超出管理范畴
            if (palive_pages += 1; require_size_index >= blocks_per_page){
                return inc::malloc((require_size_index + 1) * scale_one);
            }

            auto meta                   = inc::malloc_aligned(page_bytes, page_align_size);
            auto page                   = xnew(meta) page_header(this/*owner*/);
            auto first                  = page->first_block();

            if (page_list == nullptr){
                page_list               = page;
                page->next              = page->previous = page;
            }
            else{
                auto head               = page_list;
                head->previous->next    = page;
                page->previous          = head->previous;
                head->previous          = page;
                page->next              = head;
            }
            return split(first, blocks_per_page, require_size_index);
        }

        voidp take_out(uxx closest_index, uxx require_size_index, indicator_t & slot, node ** free_list){
            auto header = free_list[closest_index];
            auto next   = header->next;

            // 若当前元素时该链表中最后一个元素，则将该位置的 slot 设置为空
            if (next == header){
                slot.reset(closest_index);
            }
            // 否则让下一个元素顶替 header 的位置
            else{
                next->previous           = header->previous;
                header->previous->next   = next;
                free_list[closest_index] = next;
            }

            if (free_list == free_list_array){
                return split(header, closest_index + 1, require_size_index);
            }
            else{
                return split(header, node_plusp(header)->blocks, require_size_index);
            }
        }

        voidp split(nodep current, uxx total_size, uxx require_size_index){
            if (get_page_header_by(current).mark_in_use(current, require_size_index + 1);
                total_size > require_size_index + 1){
                auto require_size    = require_size_index + 1;
                auto rest            = current + require_size;
                auto rest_size       = total_size - require_size;
                xdebug(im_utils_tiny_allocator_split, current, rest, total_size, require_size, rest_size);
                xdebug_fail(rest_size > total_size);
                append(rest, rest_size);
            }
            return current;
        }

        void append(node * block, uxx index, indicator_t & slot, node ** free_list_array){
            xdebug(im_utils_tiny_allocator_append, index, slot.get(index));

            if (slot.get(index) == 0){
                slot.set(index);
                free_list_array[index]  = block;
                block->next             = block;
                block->previous         = block;
            }
            else{
                auto header             = free_list_array[index];
                block->previous         = header->previous;
                block->next             = header;
                header->previous->next  = block;
                header->previous        = block;
            }
        }

        void append(node * rest, uxx rest_size){
            auto rest_size_index = rest_size - 1;

            if (get_page_header_by(rest).set_rest(rest, rest_size); rest_size_index < boundary){
                append(rest, rest_size_index, slot, free_list_array);
            }
            else {
                append(rest, rest_size_index / boundary - 1, slot_plus, free_list_array_plus);
            }
        }

        void remove(nodep block, uxx index, indicator_t & slot, nodep * free_list){
            auto next = block->next;
            auto prev = block->previous;
            xdebug(im_utils_tiny_allocator_remove, block, next, index);

            if (next == block){
                slot.reset(index);
                return;
            }
            if (block == free_list[index]){
                free_list[index] = next;
            }
            next->previous = prev;
            prev->next     = next;
        }

        void remove(node * block, uxx block_size){
            if (auto block_size_index = block_size - 1; block_size_index < boundary){
                remove(block, block_size_index, slot, free_list_array);
            }
            else{
                remove(block, block_size_index / boundary - 1, slot_plus, free_list_array_plus);
            }
        }

        page_header & get_page_header_by(node * ptr){
            // 因为在 ptr 块都是来自 64KB 地址对齐的页，所以可以通过屏蔽低 16bit 来得到页首地址
            auto begin = uxx(ptr) & mask_to_get_header;
            return *((page_header *)begin);
        }
    };
}

#endif

xexport_space(mixc::utils_private_tiny_allocator::origin)