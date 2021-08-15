#ifndef xpack_utils_private_memory_flow
#define xpack_utils_private_memory_flow
#pragma push_macro("xuser") 
#undef  xuser
#define xuser mixc::utils_private_memory_flow::inc
#include"configure/platform.hpp"
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_fetch_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_fetch_sub.hpp"
#include"concurrency/lock/atom_fetch_xor.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
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
#include"macro/xinit.hpp"
#include"macro/xref.hpp"
#include"memop/cast.hpp"
#include"utils/memory.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_private_memory_flow::origin{
    struct memory_flow;
}

namespace mixc::utils_private_memory_flow{
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

    using indicator_t = inc::bits_indicator<scale>;

    struct meta_token;
    struct base_info{
        uxx             i_enter{};
        uxx             i_exit{};
        node_free       head{};
        node_free     * tail = & head;
        meta_token    * owner{};
        uxx             used_bytes{};
        uxx             active_object_count{};
        uxx             async_returned{};
    };

    struct page_header;
    struct meta_token : private base_info{
        using tap               = origin::memory_flow *;

        meta_token(meta_token * owner){
            inc::atom_store(xref(this->owner), owner);
        }

        void raise_change_ownership(meta_token * back_owner) {
            for(auto current = 
                inc::atom_fetch_or(xref(i_enter), 1);; // 设置 bit0 表示进入分配器将进入托管态
                inc::thread_self::yield()){

                // 等待早于该线程的其他线程完成推送任务
                if (inc::atom_load(xref(i_exit)) == current){
                    inc::atom_store(xref(owner), back_owner);   // 先设置 owner
                    back_owner->push_async_core(this);
                    inc::atom_fetch_xor(xref(i_enter), 1);      // 后清除 i_enter bit0
                    break;
                }
            }
        }

        /**
         * @brief 
         * 异步释放内存，从其他线程归还内存
         * @param mem 释放的内存，要求大小是 struct node 的整数倍
         * @param bytes 
         */
        void push_async(voidp mem, uxx bytes){
            uxx current             = {};
            for(current = inc::atom_fetch_add(xref(i_enter), 2); 
                current & 1; // 等待 raise_change_ownership 完成交接
                current = inc::atom_load(xref(i_enter))){
                inc::thread_self::yield();
            }

            auto token              = inc::atom_load(xref(owner));
            token->push_async_core(mem, bytes);
            inc::atom_fetch_add(xref(i_exit), 2);
        }

        void push_async_core(meta_token * old_token){
            if (xref(old_token->head) == old_token->tail) {
                return;
            }

            auto prev               = inc::atom_swap(xref(this->tail), old_token->tail);
            inc::atom_store(xref(prev->next), old_token->head.next);
        }

        void push_async_core(voidp mem, uxx bytes) {
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

        template<class free_t>
        void handler_async_returned(bool all_returned, free_t && free){
            // 先读取 head
            auto curr               = inc::atom_load(xref(this->head.next));
            auto next               = node_freep{};
            auto bytes              = uxx{};
            auto count              = uxx{};

            if (curr == nullptr){
                return;
            }

            while(true){
                if (next = inc::atom_load(xref(curr->next)); next == nullptr){
                    inc::atom_store(xref(this->head.next), curr);
                    break;
                }

                bytes               = inc::atom_load(xref(curr->bytes));
                free(curr, bytes);
                curr                = next;
                count              += 1;
            }

            if (all_returned){
                bytes               = inc::atom_load(xref(curr->bytes));
                free(curr, bytes);
            }
            else{
                inc::atom_fetch_sub(xref(async_returned), count);
            }
        }

        template<class free_t>
        void handler_async_returned(free_t && free){
            this->handler_async_returned(false, free);
        }

        uxx alive_object(){
            return active_object_count;
        }

        uxx alloced_bytes(){
            return used_bytes;
        }

        bool is_all_returned(){
            auto async_returned     = inc::atom_load(xref(this->async_returned));
            auto active_object      = active_object_count;
            auto all_returned       = async_returned == active_object;
            return all_returned;
        }

    public:
        uxx             pages{};
        uxx             skip_count{};
        indicator_t     slot{};
        indicator_t     slot_plus{};
        page_header   * page_list{};
        node          * free_list_array[scale];
        node          * free_list_array_plus[scale];
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
        uxx                         thread_id{};
    } * page_headerp;

    enum : uxx{
        // 一个页一共有多少可用块
        blocks_per_page         = (page_bytes - sizeof(page_header)) / scale_one,
    };

    inline page_header::page_header(meta_token * token){
        inc::atom_store(xref(this->token), token);

        // 设置哨兵位
        set(uxx(-1));
        set(blocks_per_page);
    }

    // 必须是 2 的次幂
    static_assert((page_bytes & (page_bytes - 1)) == 0);
}

namespace mixc::utils_private_memory_flow::origin{
    struct memory_flow{
    private:
        static inline
        meta_token * back;
        meta_token * token;

        friend void mem_execute();
    public:

        friend meta_token;
        friend page_header;

        ~memory_flow(){
            if (token == nullptr) {
                return;
            }

            if (auto all_returned = 
                token->is_all_returned(); all_returned){
                token->handler_async_returned(all_returned, [this](voidp ptr, uxx bytes){
                    this->free_core(ptr, bytes);
                });
            }
            else{
                token->raise_change_ownership(inc::atom_load(xref(back)));
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
            // 由于 memory_flow 是以 thread_local 变量的方式存在全局
            // 如果在它构造前有其他全局变量在初始化时调用 memory_flow::alloc 可能出现未定义行为
            if (token == nullptr){
                token               = (meta_token *)inc::memory::malloc(sizeof(meta_token));
                token               = (xnew(token) meta_token(token));
            }

            auto i_expect           = (bytes - 1) / scale_one;

            // 超出管理范畴
            if (i_expect >= blocks_per_page){
                return inc::memory::malloc((i_expect + 1) * scale_one);
            }

            // 只对 memory_flow 管理的内存计数
            token->alloc_counting(bytes);
            this->async_pop_lazily();

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

            auto i_expect           = (bytes - 1) / scale_one; 

            // 超出管理的大小
            if (i_expect >= blocks_per_page){
                inc::memory::mfree(ptr);
                return;
            }

            auto current            = nodep(ptr);
            auto head               = get_page_header_by(current);
            auto o_token            = head->token;
            auto curr               = head;

            // 处理其他线程归还的内存：
            // 如果 o_token 指向 this->token
            // 表明本线程还活着，此操作是当前线程自己回收内存
            // 也就一定没有发生所有者转换，自然也就无需通过原子操作加载
            if (this->token == o_token) {
                this->async_pop_lazily();
                this->free_core(ptr, bytes);
                return;
            }
            else{
                o_token->push_async(ptr, bytes);
            }
        }

        void handle_async_memory_event(){
            if (this->token == nullptr) {
                return;
            }
            this->async_pop_lazily();
        }

    private:
        void free_core(voidp ptr, uxx bytes){
            auto   current          = (nodep)ptr;
            auto   head             = (get_page_header_by(current));
            auto   i_expect         = (bytes - 1) / scale_one;
            auto   free_block       = (current);
            auto   free_size        = (i_expect + 1);
            auto   left             = (head->left_free_block_of(current));
            auto   right            = (head->right_free_block_of(current, free_size));

            // 只对 memory_flow 管理的内存计数
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

            inc::atom_fetch_sub(xref(token->pages), 1);

            if (page->~page_header(), inc::memory::mfree_aligned(page); next == page){
                token->~meta_token();
                inc::memory::mfree(token);
                inc::atom_store(xref(token), nullptr);
                return;
            }
            if (page == token->page_list){
                token->page_list    = next;
            }

            next->prev              = prev;
            prev->next              = next;
        }

        voidp origin_alloc(uxx i_expect){
            auto meta               = inc::memory::malloc_aligned(page_bytes, page_align_size);
            auto page               = xnew(meta) page_header(token);
            auto first              = page->first_block();

            inc::atom_fetch_add(xref(token->pages), 1);

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
                xdebug(im_utils_memory_flow_split, current, rest, total_blocks, require_blocks, rest_size);
                xdebug_fail(rest_size > total_blocks);
                append(rest, rest_size);
            }
            return current;
        }

        void append(node * block, uxx index, indicator_t & slot, node ** free_list_array){
            xdebug(im_utils_memory_flow_append, index, slot.get(index));

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
            xdebug(im_utils_memory_flow_remove, block, next, index);

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

        static page_header * get_page_header_by(voidp ptr){
            // 因为在 ptr 块都是来自 64KB 地址对齐的页，所以可以通过屏蔽低 16bit 来得到页首地址
            auto begin = uxx(ptr) & mask_to_get_header;
            return (page_header *)begin;
        }
    };

    inline void mem_execute(){
        // 只使用其中一部分
        auto && info    = base_info{};
        auto    meta    = xnew(& info) meta_token((meta_token *)& info);
        inc::atom_store(xref(memory_flow::back), meta);

        while(true){
            memory_flow::back->handler_async_returned([](voidp ptr, uxx bytes){
                auto   page     = memory_flow::get_page_header_by(ptr);
                auto   token    = inc::atom_load(xref(page->token));
                auto & mem      = inc::cast<memory_flow>(token);
                mem.free_core(ptr, bytes);
            });
            inc::thread_self::sleep(1);
        }
    }

    inline inc::thread      mem_thread;

    xinit(inc::the_mem){
        mem_thread = inc::thread(xdetached{
            mem_execute();
        });
    };
}

#endif

xexport_space(mixc::utils_private_memory_flow::origin)
