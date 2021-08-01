#ifndef xpack_utils_private_tiny_allocator
#define xpack_utils_private_tiny_allocator
#pragma push_macro("xuser") 
#undef  xuser
#define xuser mixc::utils_private_tiny_allocator::inc
#include"configure/platform.hpp"
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_fetch_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_fetch_sub.hpp"
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
#include"macro/xref.hpp"
#include"utils/allocator.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_private_tiny_allocator::origin{
    struct tiny_allocator;
}

namespace mixc::utils_private_tiny_allocator{
    typedef struct node{
        node      * prev;
        node      * next;

        #if xis_os32
    private:
        uxx         paddingx[2];
        #endif
    } * nodep;

    typedef struct node_free{
        node_free * next;
        uxx         bytes;

        #if xis_os32
    private:
        uxx         paddingx[2];
        #endif
    } * node_freep;

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

    typedef struct tiny_token{
        using tap               = origin::tiny_allocator *;

        void raise_change_owners(){
            // 这里使用原子加法避免读后写问题
            // 这里只是为了刷新缓存
            inc::atom_fetch_add(xref(used_bytes), 0);
            inc::atom_fetch_add(xref(active_object_count), 0);
            inc::atom_fetch_add(xref(async_returned), 0);

            // 后设置 need_change_ownership
            inc::atom_store(xref(need_change_ownership), true);
        }

        bool need_take_over(){
            return inc::atom_swap(xref(need_change_ownership), false);
        }

        void is_complete_handover(bool value){
            inc::atom_store(xref(handover), value);
        }

        bool is_complete_handover(){
            return inc::atom_load(xref(handover));
        }

        /**
         * @brief 
         * 异步释放内存，从其他线程归还内存
         * @param mem 释放的内存，要求大小是 struct node 的整数倍
         * @param bytes 
         */
        void push_async(voidp mem, uxx bytes){
            auto self               = node_freep(mem);
            auto prev               = node_freep(nullptr);
            inc::atom_store(xref(self->next), nullptr);
            inc::atom_store(xref(self->bytes), bytes);
            prev                    = inc::atom_swap(xref(this->tail), self);
            inc::atom_store(xref(prev->next), self);
            inc::atom_fetch_add(xref(async_returned), 1);
        }

        void alloc_counting(uxx bytes){
            active_object_count    += 1;
            used_bytes             += bytes;
        }

        void free_counting(uxx bytes, uxx count = 1){
            active_object_count    -= count;
            used_bytes             -= bytes;   
        }

        void async_return(uxx count){
            inc::atom_fetch_sub(xref(async_returned), count);
        }

        template<class free_t>
        void handler_async_returned(free_t && free){
            // 先读取 head
            auto curr               = inc::atom_load(xref(this->head.next));
            auto next               = node_freep{};
            auto bytes              = uxx{};
            auto count              = uxx{};

            if (curr == nullptr){
                return;
            }
            else {
                // 清空 this->head.next，避免下一轮读取到旧值，而实际链表是空的
                inc::atom_store(xref(this->head.next), nullptr);
            }

            // 截取 head 到 tail 这一段，做释放操作
            for(auto end = inc::atom_swap(xref(this->tail), xref(this->head));;){
                // 如果当前 node 不是链表最后一个节点，那么必须等到 next 不为空才可以释放它
                // 因为此时其他线程正在给 next 赋值
                if (curr != end) while(true) {
                    if (next = inc::atom_load(xref(curr->next)); next != nullptr){
                        break;
                    }
                    else {
                        inc::thread_self::yield();
                    }
                }

                bytes               = inc::atom_load(xref(curr->bytes));
                count              += 1;
                free(curr, bytes);

                if (curr == end){
                    break;
                }
                else {
                    curr            = next;
                }
            }

            this->async_return(count);
        }

        bool all_returned(){
            return this->active_object_count <= inc::atom_load(xref(this->async_returned));
        }

        uxx alive_object(){
            return active_object_count;
        }

        uxx alloced_bytes(){
            return used_bytes;
        }

        void merge(tiny_token * old_token){
            used_bytes             += inc::atom_load(xref(old_token->used_bytes));
            active_object_count    += inc::atom_load(xref(old_token->active_object_count));
            async_returned         += inc::atom_load(xref(old_token->async_returned));
        }

    private:
        node_free       head{};
        node_free     * tail = & head;
        uxx             used_bytes{};
        uxx             active_object_count{};
        uxx             async_returned{};
        bool            need_change_ownership{};
        bool            handover{};
    } * tiny_tokenp;

    using indicator_t   = inc::bits_indicator<scale>;

    struct page_header;

    struct tiny_allocator_meta_data {
        indicator_t     slot;
        indicator_t     slot_plus;
        uxx             skip_count              = 0;
        page_header   * page_list               = nullptr;
        node          * free_list_array[scale];
        node          * free_list_array_plus[scale];
    };

    struct meta_token : tiny_allocator_meta_data, tiny_token {
        using tiny_token::tiny_token;
    };

    typedef struct page_header{
    private:
        u08                         bmp[max_bits / 8] = {0};
        struct pair{ nodep begin = nullptr; uxx length = 0; };
    public:
        page_header(meta_token * token);

        bool get(uxx index){
            index += 1;
            return inc::bit_test(bmp[index >> 3], index & 0x7);
        }

        // 标记占用
        void set(uxx index){
            index += 1;
            inc::bit_test_and_set(xref(bmp[index >> 3]), index & 0x7);
        }

        // 标记空闲
        void reset(uxx index){
            index += 1;
            inc::bit_test_and_reset(xref(bmp[index >> 3]), index & 0x7);
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
            return nodep(this + 1);
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

        page_header               * prev{};
        page_header               * next{};
        meta_token                * token{};
        uxx                         active_return{};
    } * page_headerp;

    enum : uxx{
        // 一个页一共有多少可用块
        blocks_per_page         = (page_bytes - sizeof(page_header)) / scale_one,
    };

    inline page_header::page_header(meta_token * token) :
        token(token){

        // 设置哨兵位
        set(uxx(-1));
        set(blocks_per_page);
    }

    // 必须是 2 的次幂
    static_assert((page_bytes & (page_bytes - 1)) == 0);
}

namespace mixc::utils_private_tiny_allocator::origin{
    struct tiny_allocator{
    private:
        meta_token * token;
    public:
        friend tiny_token;

        ~tiny_allocator(){
            if (token == nullptr) {
                return;
            }
            else{
                token->raise_change_owners();
            }

            // 如果所有属于本 tiny_allocator 的对象都归还了的话
            // 就可以自行释放
            if (token->all_returned()){
                token->handler_async_returned([this](voidp ptr, uxx bytes){
                    this->free_core(ptr, bytes);
                });
                inc::mfree(token);
            }
            // 否则需要转移所有权
            else{
                token->is_complete_handover(true);
            }
        }

        uxx used_bytes() {
            return token ? token->alloced_bytes() : 0;
        }

        uxx alive_object() {
            return token ? token->alive_object() : 0;
        }

        voidp alloc(uxx bytes){
            xdebug_fail(bytes == 0){
                return nullptr;
            }

            // 注意：=============================================================================
            // 需要使用 xnew 通过构造函数初始化
            // 由于 tiny_allocator 是以 thread_local 变量的方式存在全局
            // 如果在它构造前有其他全局变量在初始化时调用 tiny_allocator::alloc 可能出现未定义行为
            if (token == nullptr){
                token               = xnew(inc::malloc(sizeof(meta_token))) meta_token();
            }

            auto i_expect           = (bytes - 1) / scale_one;

            // 超出管理范畴
            if (i_expect >= blocks_per_page){
                return inc::malloc((i_expect + 1) * scale_one);
            }

            // 只对 tiny_allocator 管理的内存计数
            token->alloc_counting(bytes);

            // slot 中置位位表示空闲的块
            // 选择最接近但不小于所需大小的块
            if (i_expect < boundary){
                if (auto i_closest = token->slot.index_of_first_set(i_expect); i_closest != not_exist){
                    return take_out(i_closest, i_expect, token->slot, token->free_list_array);
                }
            }
            if (i_expect < blocks_per_page){
                if (auto i_closest = token->slot_plus.index_of_first_set(i_expect / boundary); i_closest != not_exist){
                    return take_out(i_closest, i_expect, token->slot_plus, token->free_list_array_plus);
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

            // 处理其他线程归还的内存：
            // 如果 o_token 指向 this->token
            // 表明本线程还活着，此操作是当前线程自己回收内存
            // 也就一定没有发生所有者转换，自然也就无需通过原子操作加载
            if (o_token == this->token) {
                this->async_pop_lazily();
                this->free_core(ptr, bytes);
                return;
            }

            // 先设置计数器
            inc::atom_fetch_add(xref(head->active_return), 1);

            // 后读取 token
            o_token                 = inc::atom_load(xref(head->token));

            // 可以正常异步推送可释放的内存
            if (o_token->need_take_over() == false){
                o_token->push_async(ptr, bytes);

                // 推送完成后再让计数器减一
                inc::atom_fetch_sub(xref(head->active_return), 1);
            }
            // o_token 所有者发出交接信号，第一个看到信号的线程作为新的所有者
            else{
                auto curr           = head; 

                // 获取到锁后让计数器减一，避免下方 do-while 循环自己等待自己
                inc::atom_fetch_sub(xref(head->active_return), 1);

                // 待优化 ====================================================
                do{
                    // 更改 page 链表所有 token 的指向
                    inc::atom_store(xref(curr->token), this->token);

                    // 等待异步推送完毕，才可以释放 o_token
                    while(inc::atom_load(xref(curr->active_return)) != 0){
                        inc::thread_self::yield();
                    }

                    curr            = curr->next;
                }while(curr != head);

                // 合并位指示器
                this->merge(
                    o_token->slot, 
                    o_token->free_list_array, 
                    token->slot, 
                    token->free_list_array
                );
                this->merge(
                    o_token->slot_plus, 
                    o_token->free_list_array_plus, 
                    token->slot_plus, 
                    token->free_list_array_plus
                );

                // 合并 token 内部计数器
                token->merge(o_token);

                // 合并 page
                if (token->page_list == nullptr){
                    token->page_list = o_token->page_list;
                }
                else{
                    auto o_head     = o_token->page_list;
                    auto o_end      = o_head->prev;
                    auto n_head     = token->page_list;
                    auto n_next     = page_headerp{};
                    n_next          = n_head->next;
                    n_head->next    = o_head;
                    n_next->prev    = o_end;
                    o_head->prev    = n_head;
                    o_end->next     = n_next;
                }

                // 可以一次释放所有异步推送的内存，因为之前在更改 token 时等待异步推送
                o_token->handler_async_returned([this](voidp ptr, uxx bytes){
                    this->free_core(ptr, bytes);
                });

                // 现在 ptr 已经属于当前线程了
                // 我们无需也不应该使用 o_token->push_async(ptr, bytes);
                // 因为异步推送后会让 o_token 的计数器+1 而不是让 this->token 的计数器+1
                // 之前已经通过 token->merge(o_token); 合并了计数器
                // 所以这里不再往旧的 o_token 推送可释放的内存
                this->free_core(ptr, bytes);

                while (o_token->is_complete_handover() == false) {
                    inc::thread_self::yield();
                }

                // 放到最后等待
                inc::mfree(o_token);
            }
        }

        void process_message(){
            if (this->token == nullptr) {
                return;
            }
            this->async_pop_lazily();
        }

    private:
        void merge(indicator_t & old_slot, node ** old_free_list_array, indicator_t & new_slot, node ** new_free_list_array){
            for(uxx i = 0;; i = i + 1){
                if (i = old_slot.index_of_first_set(i/*从该位开始寻找第一个置位位*/); i == not_exist){
                    break;
                }

                auto o_head         = old_free_list_array[i];
                auto o_end          = o_head->prev;
                auto n_head         = new_free_list_array[i];
                auto n_next         = nodep{};

                if (new_slot.get(i)){
                    n_next          = n_head->next;
                    o_head->prev    = n_head;
                    o_end->next     = n_next;
                    n_head->next    = o_head;
                    n_next->prev    = o_end;
                    continue;
                }

                new_slot.set(i);
                new_free_list_array[i]
                                    = o_head;
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

            // 只对 tiny_allocator 管理的内存计数
            if (token->free_counting(bytes),
                head->mark_free(current, i_expect + 1); left.begin != nullptr){
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
            }
            // 否则合成的更大的空闲块放回对应的链表数组中
            else{
                append(free_block, free_size);
            }
        }

        void async_pop_lazily(){
            // 忽略一定次数后就响应一下
            // 尽可能减少无效的原子操作
            if (token->skip_count += 1; token->skip_count == max_skip_times){
                token->skip_count   = 0;

                token->handler_async_returned([this](voidp ptr, uxx bytes){
                    this->free_core(ptr, bytes);
                });
            }
        }

        void origin_free(page_header * page){
            auto next               = page->next;
            auto prev               = page->prev;

            if (page->~page_header(), inc::mfree_aligned(page); next == page){
                token->page_list    = nullptr;
                return;
            }
            if (page == token->page_list){
                token->page_list    = next;
            }

            next->prev              = prev;
            prev->next              = next;
        }

        voidp origin_alloc(uxx i_expect){
            auto meta               = inc::malloc_aligned(page_bytes, page_align_size);
            auto page               = xnew(meta) page_header(token);
            auto first              = page->first_block();

            if (token->page_list == nullptr){
                token->page_list    = page;
                page->next          = page->prev = page;
            }
            else{
                auto head           = token->page_list;
                auto prev           = head->prev;
                prev->next          = page;
                page->prev          = head->prev;
                head->prev          = page;
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
                next->prev              = first->prev;
                first->prev->next       = next;
                free_list[i_closest]    = next;
            }

            if (free_list == token->free_list_array){
                return split(first, i_closest + 1, i_expect);
            }
            else{
                return split(first, node_plusp(first)->blocks, i_expect);
            }
        }

        voidp split(nodep current, uxx total_blocks, uxx i_expect){
            if (get_page_header_by(current)->mark_in_use(current, i_expect + 1);
                total_blocks > i_expect + 1){
                auto require_blocks     = i_expect + 1;
                auto rest               = current + require_blocks;
                auto rest_size          = total_blocks - require_blocks;
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
                block->prev             = block;
            }
            else{
                auto header             = free_list_array[index];
                block->prev             = header->prev;
                block->next             = header;
                header->prev->next      = block;
                header->prev            = block;
            }
        }

        void append(node * rest, uxx rest_size){
            auto rest_size_index = rest_size - 1;

            if (get_page_header_by(rest)->set_rest(rest, rest_size); rest_size_index < boundary){
                append(rest, rest_size_index, token->slot, token->free_list_array);
            }
            else {
                append(rest, rest_size_index / boundary - 1, token->slot_plus, token->free_list_array_plus);
            }
        }

        void remove(nodep block, uxx index, indicator_t & slot, nodep * free_list){
            auto next = block->next;
            auto prev = block->prev;
            xdebug(im_utils_tiny_allocator_remove, block, next, index);

            if (next == block){
                slot.reset(index);
                return;
            }
            if (block == free_list[index]){
                free_list[index] = next;
            }
            next->prev      = prev;
            prev->next      = next;
        }

        void remove(node * block, uxx block_size){
            if (auto block_size_index = block_size - 1; block_size_index < boundary){
                remove(block, block_size_index, token->slot, token->free_list_array);
            }
            else{
                remove(block, block_size_index / boundary - 1, token->slot_plus, token->free_list_array_plus);
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
