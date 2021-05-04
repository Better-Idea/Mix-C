#ifndef xpack_utils_private_tiny_allocator
#define xpack_utils_private_tiny_allocator 
#pragma push_macro("xuser")  
#undef  xuser
#define xuser mixc::utils_private_tiny_allocator::inc
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_fetch_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
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

        wait_change_owners      = 1 << 0,
        mask_to_get_self_id     = ~(wait_change_owners),
        i_new_push              = 1,
        new_push                = 1 << i_new_push,
    };

    /* 命名：
     * 我们称 token 原先的所有者为[故主]，新持有者为[新秀]
     * 
     * 过程：
     * - [故主]通过 raise_change_owners 发起变更所有者[公告]
     * - 有些线程 try_push 时是没有读到置位的 wait_change_owners 位，指示它们没有看到[公告]
     *   此时它们成为 escaped [逃逸线程]
     * - 有些线程看到了[公告]，此时它们等待[故主]通过设置 i_new_owner 标识钦定[新秀]
     * - [故主]在设置 wait_change_owners 位时会得到最后一个[逃逸线程]的 id
     *   那么该 id 后一个就是第一个看到[公告]的线程，此时该 id 对应的线程被选为[新秀]
     * - try_push 会返回当前线程的操作 self_id，该值是每执行一次 try_push 就增一
     *   所以各个线程能得到独有的 id
     * - 当[故主]设置好 i_new_owner 后，之前等待的线程会比对自己是不是被钦定的[新秀]
     * - 如果自己成为[新秀]，需要将 powner 指向自己，并复位 wait_change_owners 位
     *   指示交接以完成，[新秀]需要接替[故主]内存回收工作，并等待[逃逸线程]完成异步推送
     * - 其他线程则是等待交接完成再将释放的内存推送给[新秀]
     * 
     * 补充：
     * 这里不让线程完成 push 后使计数器减一，因为这样会导致 id 不唯一
     */
    typedef struct tiny_token{
        using tap               = origin::tiny_allocator *;

        bool can_release(){
            return i_ref == 0;
        }

        auto try_push(){
            struct { uxx self_id; bool wait_change_owners; } i;
            uxx mask            = inc::atom_fetch_add(xref state, new_push);
            i.self_id           = (mask & mask_to_get_self_id);
            i.wait_change_owners= (mask & wait_change_owners) != 0;
            return i;
        }

        void wait_escaped_finish(){
            // 等到所有[逃逸线程]完成操作
            while(inc::atom_load(xref i_finished) + new_push != i_new_owner){
                inc::thread_self::yield();
            }
        }

        void raise_change_owners(){
            inc::atom_store(xref i_new_owner, not_exist);
            auto fetch          = inc::atom_fetch_or(
                xref state, wait_change_owners
            );
            auto new_owner      = fetch & mask_to_get_self_id;

            // 让以下字段的改动对其他线程可见
            inc::atom_load(xref alive_pages);
            inc::atom_load(xref used_bytes);
            inc::atom_load(xref alive_object);

            // 之前的字段设置完毕，可以设置 i_new_owner，其他线程可能在等待它被赋予新值
            inc::atom_store(xref i_new_owner, new_owner);
        }

        void async_push(voidp mem, uxx bytes){
            auto self               = node_freep(mem);
            auto prev               = node_freep(nullptr);
            inc::atom_store(xref self->next, nullptr);
            inc::atom_store(xref self->bytes, bytes);
            prev                    = inc::atom_swap(xref async_tail, self);
            inc::atom_store(xref prev->next, self);
            inc::atom_fetch_add(xref i_async_push, 1);

            // 完成推送，最后设置该字段
            inc::atom_fetch_add(xref i_finished, 1);
        }

        tap             owner{};
        uxx             state{};
        uxx             i_ref{};
        uxx             i_finished{};
        uxx             i_new_owner{};
        uxx             i_async_push{};
        uxx             alive_pages{};
        uxx             used_bytes{};
        uxx             alive_object{};
        node_free       async_head{};
        node_free     * async_tail = xref async_head;
    } * tiny_tokenp;

    typedef struct page_header{
    private:
        u08                         bmp[max_bits / 8] = {0};
        struct pair{ nodep begin = nullptr; uxx length = 0; };
    public:
        page_header(tiny_token * token);

        ~page_header(){
            token->i_ref           -= 1;
        }

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
        tiny_token                * token;
        uxx                         reserved;
    } * page_headerp;

    enum : uxx{
        // 一个页一共有多少可用块
        blocks_per_page         = (page_bytes - sizeof(page_header)) / scale_one,
    };

    inline page_header::page_header(tiny_token * token) :
        token(token){
        token->i_ref           += 1;

        // 设置哨兵位
        set(uxx(-1));
        set(blocks_per_page);
    }

    // 必须是 2 的次幂
    static_assert((page_bytes & (page_bytes - 1)) == 0);

    struct tiny_allocator_header{
        using indicator_t   = inc::bits_indicator<scale>;

        indicator_t     slot;
        indicator_t     slot_plus;
        uxx             skip_count              = 0;
        page_header   * page_list               = nullptr;
        tiny_token    * token                   = nullptr;
        node          * free_list_array[scale];
        node          * free_list_array_plus[scale];
    };
}

namespace mixc::utils_private_tiny_allocator::origin{
    struct tiny_allocator : tiny_allocator_header{
        friend tiny_token;

        tiny_allocator(){
            token                   = tiny_tokenp(inc::malloc(sizeof(tiny_token)));
            inc::atom_store(xref token->owner, this);
        }

        ~tiny_allocator(){
            // 如果其他线程归还所有分配的内存就自行析构
            if (inc::atom_load(xref token->i_async_push) == token->alive_object){
                for(auto head = token->async_head.next; head != nullptr;){
                    auto next       = inc::atom_load(xref head->next);
                    auto bytes      = inc::atom_load(xref head->bytes);
                    this->free_core(head, bytes);
                    head            = next;
                }
                token->alive_object = 0;
            }
            if (token->alive_object == 0){
                inc::mfree(token);
                return;
            }
            // 否则需要转移所有者
            else{
                token->raise_change_owners();
                token->wait_escaped_finish();
            }
        }

        uxx used_bytes() {
            return token->used_bytes;
        }

        uxx alive_object() {
            return token->alive_object;
        }

        uxx alive_pages(){
            return token->alive_pages;
        }

        voidp alloc(uxx bytes){
            xdebug_fail(bytes == 0){
                return nullptr;
            }

            auto i_expect            = (bytes - 1) / scale_one;

            // 超出管理范畴
            if (i_expect >= blocks_per_page){
                return inc::malloc((i_expect + 1) * scale_one);
            }

            token->used_bytes      += bytes;
            token->alive_object    += 1;

            // slot 中置位位表示空闲的块
            // 选择最接近但不小于所需大小的块
            if (i_expect < boundary){
                if (auto i_closest = slot.index_of_first_set(i_expect); i_closest != not_exist){
                    return take_out(i_closest, i_expect, slot, free_list_array);
                }
            }
            if (i_expect < blocks_per_page){
                if (auto i_closest = slot_plus.index_of_first_set(i_expect / boundary); i_closest != not_exist){
                    return take_out(i_closest, i_expect, slot_plus, free_list_array_plus);
                }
            }
            return origin_alloc(i_expect);
        }

        void free(voidp ptr, uxx bytes){
            xdebug_fail(bytes == 0){
                return;
            }

            auto   i_expect         = (bytes - 1) / scale_one; 

            // 超出管理的大小
            if (i_expect >= blocks_per_page){
                inc::mfree(ptr);
                return;
            }

            auto   current          = nodep(ptr);
            auto   head             = get_page_header_by(current);
            auto   o_token          = head->token;
            auto   owner            = o_token->owner;

            // 处理其他线程归还的内存：
            // 如果 owner 指向 this
            // 表明本线程还活着，此操作是当前线程自己回收内存
            // 也就一定没有发生所有者转换，自然也就无需通过原子操作加载
            if (owner == this){
                this->async_pop_lazily();
                this->free_core(ptr, bytes);
                return;
            }
            // 如果 owner 不指向 this，情况就变复杂了
            // 它可能正在或者已经变更所有者
            // 出于并发性能考虑，在转移所有者时，我们不会更改 [故主] 的 page->token
            // 所以有下面峰回路转的加载操作
            else{
                o_token             = inc::atom_load(xref head->token);
                owner               = inc::atom_load(xref o_token->owner);
                o_token             = inc::atom_load(xref owner->token);
            }

            using tap               = tiny_allocator *;
            auto i                  = o_token->try_push();
            auto inherit            = [&](){
                // 更改所有者
                auto o_owner        = inc::atom_swap(xref o_token->owner, this);
                auto list           = head;

                // 其他相关线程可能会等待该位复位
                // 本来在这之前需要改变 [故主] 所有 page->token
                // 但如果 [故主] 遗留的 page 较多会导致所有向 [故主] 归还内存的线程都被阻塞
                // 直到 o_token->state 中的 wait_change_owners 位被复位
                inc::atom_fetch_and(xref o_token->state, ~wait_change_owners);
                this->merge(o_owner);
                this->free_core(ptr, bytes);

                // 原子操作读取方可在带缓存的 cpu 中读到改动
                token->used_bytes  += inc::atom_load(xref o_token->used_bytes);
                token->alive_pages += inc::atom_load(xref o_token->alive_pages);
                token->alive_object+= inc::atom_load(xref o_token->alive_object);

                // 等到所有[逃逸线程]完成操作
                o_token->wait_escaped_finish();

                auto head           = inc::atom_load(xref o_token->async_head.next);
                auto next           = node_freep{};
                auto bytes          = uxx{};
                inc::atom_fetch_add(xref o_token->i_finished, new_push);

                while(head != nullptr){
                    bytes           = inc::atom_load(xref head->bytes);
                    next            = inc::atom_load(xref head->next);
                    this->free_core(head, bytes);
                    head            = next;
                }
            };

            if (i.wait_change_owners){
                while(inc::atom_load(xref o_token->i_new_owner) == not_exist){
                    inc::thread_self::yield();
                }

                if (o_token->i_new_owner == i.self_id){
                    inherit();
                    return;
                }

                while(inc::atom_load(xref o_token->state) & wait_change_owners){
                    inc::thread_self::yield();
                }

                // 重新加载
                owner               = inc::atom_load(xref token->owner);
                o_token             = inc::atom_load(xref owner->token);
            }

            // 推送给所属的线程
            // 如果此时正在发生所有者变更，那么此线程将成为[逃逸线程]
            // [故主] 和 [新秀] 会等待所有的逃逸线程完成推送
            // 而相关的后入线程会阻塞，直到 [故主] 与 [新秀] 交接完成
            o_token->async_push(ptr, bytes);
        }

        void process_message(){
            async_pop_lazily();
        }
    private:
        void async_pop(){
            auto free_size          = 0;
            auto phead              = xref token->async_head;
            auto head               = inc::atom_load(xref phead->next);
            auto next               = node_freep{};
            auto bytes              = uxx{};
            auto i                  = uxx{};

            if (head == nullptr){
                return;
            }

            xdefer{
                inc::atom_sub(xref token->i_async_push, i);
            };

            while(true){
                if (next = head->next; next == nullptr){
                    next            = inc::atom_load(xref head->next);
                }
                if (next == nullptr){
                    phead->next     = head;
                    return;
                }

                bytes               = inc::atom_load(xref head->bytes);
                this->free_core(head, bytes);
                head                = next;
            }
        }

        void merge(indicator_t & old_slot, node ** old_free_list_array){
            for(uxx i = 0;;){
                if (i = old_slot.index_of_first_set(i); i == not_exist){
                    break;
                }

                auto o_head         = old_free_list_array[i];
                auto o_end          = o_head->previous;
                auto n_head         = free_list_array[i];
                auto n_next         = nodep{};

                if (slot.get(i)){
                    n_next          = n_head->next;
                    o_head->previous= n_head;
                    n_head->next    = o_head;
                    o_end->next     = n_next;
                    n_next->previous= o_end;
                    continue;
                }

                slot.set(i);
                free_list_array[i]  = o_head;
            }
        }

        void merge(tiny_allocator * old_owner){
            merge(old_owner->slot, old_owner->free_list_array);
            merge(old_owner->slot_plus, old_owner->free_list_array_plus);

            if (page_list == nullptr){
                page_list           = old_owner->page_list;
            }
            else{
                auto o_head         = old_owner->page_list;
                auto o_end          = o_head->previous;
                auto n_head         = page_list;
                auto n_next         = page_headerp{};
                n_next              = n_head->next;
                o_head->previous    = n_head;
                n_head->next        = o_head;
                o_end->next         = n_next;
                n_next->previous    = o_end;
            }
        }

        void free_core(voidp ptr, uxx bytes){
            auto   current          = (nodep)ptr;
            auto   head             = (get_page_header_by(current));
            auto   i_expect         = (bytes - 1) / scale_one;
            auto   free_block       = (current);
            auto   free_size        = (i_expect + 1);
            auto   left             = (head->left_free_block_of(current));
            auto   right            = (head->right_free_block_of(current, free_size));
            token->used_bytes      -= (bytes);
            token->alive_object    -= (1);

            if (head->mark_free(current, i_expect + 1); left.begin != nullptr){
                head->mark_free(left.begin, left.length);
                free_block          = left.begin;
                free_size          += left.length;
                remove(left.begin, left.length);
            }
            if (right.begin != nullptr){
                free_size          += right.length;
                head->mark_free(right.begin, right.length);
                remove(right.begin, right.length);
            }

            // 整个页都空闲时就释放该页
            if (free_size == blocks_per_page){
                origin_free(head);
                return;
            }

            // 否则合成的更大的空闲块放回对应的链表数组中
            append(free_block, free_size);
        }

        void async_pop_lazily(){
            // 忽略一定次数后就响应一下
            // 尽可能减少无效的原子操作
            if (skip_count += 1; skip_count == max_skip_times){
                skip_count          = 0;
                this->async_pop();
            }
        }

        void origin_free(page_header * page){
            auto next               = page->next;
            auto prev               = page->previous;

            // 如果 page->token 不是本线程的，则该 page 其实是从[故主]那边接手过来的
            // 当 page->token 没有使用者了，则表示[故主]所有的 page 都已经释放完毕了
            // 所以无需再保留该 token
            if (page->~page_header();
                page->token != this->token and page->token->can_release()){
                page->token->~tiny_token();
                inc::mfree(token);
            }

            inc::mfree_aligned(page);

            if (token->alive_pages -= 1; next == page){
                page_list           = nullptr;
                return;
            }
            if (page == page_list){
                page_list           = next;
            }

            next->previous          = prev;
            prev->next              = next;
        }

        voidp origin_alloc(uxx i_expect){
            auto meta               = inc::malloc_aligned(page_bytes, page_align_size);
            auto page               = xnew(meta) page_header(token);
            auto first              = page->first_block();
            token->alive_pages     += 1;

            if (page_list == nullptr){
                page_list           = page;
                page->next          = page->previous = page;
            }
            else{
                auto head           = page_list;
                auto prev           = head->previous;
                prev->next          = page;
                page->previous      = head->previous;
                head->previous      = page;
                page->next          = head;
            }
            return split(first, blocks_per_page, i_expect);
        }

        voidp take_out(uxx i_closest, uxx i_expect, indicator_t & slot, node ** free_list){
            auto    first               = free_list[i_closest];
            auto    next                = first->next;

            // 若当前元素时该链表中最后一个元素，则将该位置的 slot 设置为空
            if (next == first){
                slot.reset(i_closest);
            }
            // 否则让下一个元素顶替 first 的位置
            else{
                next->previous          = first->previous;
                first->previous->next   = next;
                free_list[i_closest]    = next;
            }

            if (free_list == free_list_array){
                return split(first, i_closest + 1, i_expect);
            }
            else{
                return split(first, node_plusp(first)->blocks, i_expect);
            }
        }

        voidp split(nodep current, uxx total_blocks, uxx i_expect){
            if (get_page_header_by(current)->mark_in_use(current, i_expect + 1);
                total_blocks > i_expect + 1){
                auto require_blocks  = i_expect + 1;
                auto rest            = current + require_blocks;
                auto rest_size       = total_blocks - require_blocks;
                xdebug(im_utils_tiny_allocator_split, current, rest, total_blocks, require_blocks, rest_size);
                xdebug_fail(rest_size > total_blocks);
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

            if (get_page_header_by(rest)->set_rest(rest, rest_size); rest_size_index < boundary){
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

        page_header * get_page_header_by(node * ptr){
            // 因为在 ptr 块都是来自 64KB 地址对齐的页，所以可以通过屏蔽低 16bit 来得到页首地址
            auto begin = uxx(ptr) & mask_to_get_header;
            return (page_header *)begin;
        }
    };
}

#endif

xexport_space(mixc::utils_private_tiny_allocator::origin)
