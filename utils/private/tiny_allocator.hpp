#ifndef xpack_utils_private_tiny_allocator
#define xpack_utils_private_tiny_allocator 
#pragma push_macro("xuser")  
#undef  xuser
#define xuser mixc::utils_private_tiny_allocator::inc
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_fetch_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_fetch_xor.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/lock_state_t.hpp"
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

namespace mixc::utils_private_tiny_allocator{
    struct tiny_allocator;

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

    /* 命名：
     * 我们称 token 原先的所有者为[故主]，新持有者为[新秀]
     * 
     * 过程：
     * - [故主]通过 raise_change_owners 发起变更所有者[公告]
     * - 有些线程 try_push 时是没有读到置位的 wait_chanage_owners 位，指示它们没有看到[公告]
     *   此时它们成为 escaped [逃逸线程]
     * - 有些线程看到了[公告]，此时它们等待[故主]通过设置 i_new_owner 标识钦定[新秀]
     * - [故主]在设置 wait_chanage_owners 位时会得到最后一个[逃逸线程]的 id
     *   那么该 id 后一个就是第一个看到[公告]的线程，此时该 id 对应的线程被选为[新秀]
     * - try_push 会返回当前线程的操作 id，该值是每执行一次 try_push 就增一
     *   所以各个线程能得到独有的 id
     * - 当[故主]设置好 i_new_owner 后，之前等待的线程会比对自己是不是被钦定的[新秀]
     * - 如果自己成为[新秀]，需要将 powner 指向自己，并复位 wait_chanage_owners 位
     *   指示交接以完成，[新秀]需要接替[故主]内存回收工作，并等待[逃逸线程]完成异步推送
     * - 其他线程则是等待交接完成再将释放的内存推送给[新秀]
     * 
     * 补充：
     * 这里不让线程完成 push 后使计数器减一，因为这样会导致 id 不唯一
     */
    typedef struct tiny_token{
        using tap   = tiny_allocator *;

        enum{
            wait_chanage_owners     = 1 << 0,
            wait_escaped            = 1 << 1,
            wait_shader             = 1 << 2,
            i_new_push              = 3,
            new_push                = 1 << i_new_push,
        };

        tiny_token(tap owner) : 
            powner(owner),
            i_push_count(0),
            i_escaped_base(0),
            i_shader(0),
            i_new_owner(not_exist),
            state(0),
            ref_count(0){
        }

        auto try_push(){
            struct { inc::lock_state_t state; uxx id; } r;
            auto fetch          = inc::atom_fetch_add(xref state, new_push);
            r.id                = fetch >> i_new_push;
            r.state             = fetch & wait_chanage_owners ?
                inc::lock_state_t::accept : inc::lock_state_t::blocked;
            return r;
        }

        void raise_change_owners(){
            inc::atom_store(xref i_new_owner, not_exist);
            inc::atom_store(xref i_escaped_base, 0);
            auto fetch          = inc::atom_fetch_or(
                xref state, wait_chanage_owners | wait_escaped
            );
            auto new_owner      = fetch >> i_new_push;
            inc::atom_store(xref i_new_owner, new_owner);
        };

        template<class lambda_t>
        tap wait(uxx id, tap new_owner, lambda_t && if_selected){
            uxx i_owner         = 0;
            while((i_owner = inc::atom_load(xref i_new_owner)) == not_exist){
                inc::thread_self::yield();
            }

            // 被选中
            if (id == i_owner){
                auto old_owner  = inc::atom_swap(xref powner, new_owner);
                inc::atom_fetch_and(xref state, ~wait_chanage_owners);
                if_selected(old_owner);
                return new_owner;
            }
            else while(inc::atom_load(xref state) & wait_chanage_owners){
                inc::thread_self::yield();
            }
            return inc::atom_load(xref powner);
        }

        // 只有[新秀]需要等待[逃逸线程]
        void wait_escaped_thread(){
            while(inc::atom_load(xref i_escaped_base) != i_new_owner - 1){
                inc::thread_self::yield();
            }
            inc::atom_fetch_xor(xref state, wait_shader/*置位*/ | wait_escaped/*复位*/);
        }

        void finish_push(uxx id){
            if (auto mask = inc::atom_fetch_and(xref state, ~wait_shader); mask & wait_escaped){
                if (id < inc::atom_load(xref i_new_owner)){
                    inc::atom_fetch_add(xref i_escaped_base, 1);
                }
                else{
                    inc::atom_fetch_add(xref i_shader, 1);
                }
            }
            else {
                if (mask & wait_shader){
                    auto shader = inc::atom_swap(xref i_shader, uxx{});
                    inc::atom_fetch_add(xref i_escaped_base, shader);
                }
                else{
                    inc::atom_fetch_add(xref i_escaped_base, 1);
                }
            }
        }

        tap owner(){
            return powner;
        }

        tap owner_atom_load(){
            return inc::atom_load(xref powner);
        }

        void owner_increase(){
            ref_count += 1;
        }
        
        void owner_decrease(){
            ref_count -= 1;
        }

        bool has_owner(){
            return ref_count != 0;
        }

    private:
        tap             powner;
        uxx             i_push_count;
        uxx             i_escaped_base;
        uxx             i_shader;
        uxx             i_new_owner;
        uxx             state;
        uxx             ref_count;
    } * tiny_tokenp;

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
        page_header(tiny_token * token);

        ~page_header(){
            token->owner_decrease();
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
        token->owner_increase();

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
        uxx             async_push_bytes        = 0;
        uxx             palive_pages            = 0;
        uxx             pused_bytes             = 0;
        uxx             pneed_free_count        = 0;
        uxx             skip_count              = 0;
        page_header   * page_list               = nullptr;
        node_free       async_head              = {};
        node_free     * async_tail              = & async_head;
        tiny_token    * token                   = nullptr;
        node          * free_list_array[scale];
        node          * free_list_array_plus[scale];
    };

    struct tiny_allocator : tiny_allocator_header{
        tiny_allocator(){
            token                   = tiny_tokenp(inc::malloc(sizeof(tiny_token)));
            xnew(token) tiny_token(this/*owner*/);
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
            auto i_require_size     = (bytes - 1) / scale_one;
            pused_bytes            += (bytes);

            // slot 中置位位表示空闲的块
            // 选择最接近但不小于所需大小的块
            if (i_require_size < boundary){
                if (auto i_closest = slot.index_of_first_set(i_require_size); i_closest != not_exist){
                    return take_out(i_closest, i_require_size, slot, free_list_array);
                }
            }
            if (i_require_size < blocks_per_page){
                if (auto i_closest = slot_plus.index_of_first_set(i_require_size / boundary); i_closest != not_exist){
                    return take_out(i_closest, i_require_size, slot_plus, free_list_array_plus);
                }
            }
            return origin_alloc(i_require_size);
        }

        void free(voidp ptr, uxx bytes){
            xdebug_fail(bytes == 0){
                return;
            }

            auto   i_scale          = (bytes - 1) / scale_one; 

            // 超出管理的大小
            if (i_scale >= blocks_per_page){
                inc::mfree(ptr);
                return;
            }

            auto   current          = nodep(ptr);
            auto & head             = get_page_header_by(current);
            auto   token            = head.token;
            auto   owner            = token->owner();

            // 处理其他线程归还的内存
            if (owner == this){
                this->async_pop_lazily();
                this->free_core(ptr, bytes);
                return;
            }
            else{
                owner               = token->owner_atom_load();
            }

            auto   self             = token->try_push(); 
            auto   if_selected      = [&](tiny_allocator * old_owner){ 
                palive_pages       += old_owner->palive_pages;
                pneed_free_count   += old_owner->pneed_free_count;
                pused_bytes        += old_owner->pused_bytes;
                this->merge(old_owner);

                // 等待[逃逸线程]完成推送
                token->wait_escaped_thread();

                // 代替[故主]处理[逃逸线程]的推送
                auto head           = inc::atom_load(xref old_owner->async_head.next);
                auto next           = node_freep{};
                auto bytes          = uxx{};

                while(head != nullptr){
                    bytes           = inc::atom_load(xref head->bytes);
                    next            = inc::atom_load(xref head->next);
                    this->free_core(head, bytes);
                    head            = next;
                }
            };

            // 如果该内存属于其他线程，就推送到分配它的线程
            if (self.state == inc::lock_state_t::blocked){
                owner               = token->wait(self.id, this, if_selected);
            }
            // 分配该内存的线程可能已经退出并将所有权转交给后台内存清理线程
            // cpu 读了一次 token->owner 后可能会被缓存下来
            // 所以该改动需要原子操作才能读取到
            else{
                owner               = token->owner_atom_load();
            }

            owner->async_push(current, bytes);
            token->finish_push(self.id);
        }

        void process_message(){
            async_pop_lazily();
        }
    private:
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
                return;
            }

            auto o_head             = old_owner->page_list;
            auto o_end              = o_head->previous;
            auto n_head             = page_list;
            auto n_next             = page_headerp{};
            n_next                  = n_head->next;
            o_head->previous        = n_head;
            n_head->next            = o_head;
            o_end->next             = n_next;
            n_next->previous        = o_end;
        }

        void free_core(voidp ptr, uxx bytes){
            auto   current          = (nodep)ptr;
            auto & head             = (get_page_header_by(current));
            auto   i_scale          = (bytes - 1) / scale_one;
            auto   free_block       = (current);
            auto   free_size        = (i_scale + 1);
            auto   left             = (head.left_free_block_of(current));
            auto   right            = (head.right_free_block_of(current, free_size));
            pused_bytes            -= (bytes);
            pneed_free_count       -= (1);

            if (head.mark_free(current, i_scale + 1); left.begin != nullptr){
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
            inc::atom_store(xref self->next, nullptr);
            inc::atom_store(xref self->bytes, bytes);
            prev                    = inc::atom_swap(xref async_tail, self);
            inc::atom_store(xref prev->next, self);
            inc::atom_fetch_add(xref async_push_bytes, bytes);
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
            auto free_size          = 0;
            auto head               = inc::atom_load(xref async_head.next);
            auto next               = node_freep{};

            if (head == nullptr){
                return;
            }

            xdefer{
                inc::atom_sub(xref async_push_bytes, free_size);
            };

            while(true){
                if (next = head->next; next == nullptr){
                    next            = inc::atom_load(xref head->next);
                }
                if (next == nullptr){
                    inc::atom_store(xref async_head.next, head);
                    return;
                }

                free_size          += head->bytes;
                free_core(head, head->bytes);
                head                = next;
            }
        }

        void origin_free(page_header * page){
            auto next               = page->next;
            auto prev               = page->previous;

            // 如果 page->token 不是本线程的，则该 page 其实是从[故主]那边接手过来的
            // 当 page->token 没有使用者了，则表示[故主]所有的 page 都已经释放完毕了
            // 所以无需再保留该 token
            if (page->~page_header();
                page->token != this->token and not page->token->has_owner()){
                page->token->~tiny_token();
                inc::mfree(token);
            }

            inc::mfree_aligned(page);

            if (palive_pages -= 1; next == page){
                page_list           = nullptr;
                return;
            }
            if (page == page_list){
                page_list           = next;
            }

            next->previous          = prev;
            prev->next              = next;
        }

        voidp origin_alloc(uxx i_require_size){
            // 超出管理范畴
            if (i_require_size >= blocks_per_page){
                return inc::malloc((i_require_size + 1) * scale_one);
            }

            auto meta               = inc::malloc_aligned(page_bytes, page_align_size);
            auto page               = xnew(meta) page_header(token);
            auto first              = page->first_block();
            palive_pages           += 1;

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
            return split(first, blocks_per_page, i_require_size);
        }

        voidp take_out(uxx i_closest, uxx i_require_size, indicator_t & slot, node ** free_list){
            auto    first               = free_list[i_closest];
            auto    next                = first->next;

            // 属于本线程的内存个数
            pneed_free_count           += 1;

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
                return split(first, i_closest + 1, i_require_size);
            }
            else{
                return split(first, node_plusp(first)->blocks, i_require_size);
            }
        }

        voidp split(nodep current, uxx total_size, uxx i_require_size){
            if (get_page_header_by(current).mark_in_use(current, i_require_size + 1);
                total_size > i_require_size + 1){
                auto require_size    = i_require_size + 1;
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