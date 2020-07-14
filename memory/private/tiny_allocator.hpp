#ifndef xpack_memory_private_tiny_allocator
#define xpack_memory_private_tiny_allocator 
    #pragma push_macro("xuser")  
    #undef  xuser
    #define xuser mixc::memory_private_tiny_allocator
    #include"define/base_type.hpp"
    #include"docker/bit_indicator.hpp"
    #include"interface/ranger.hpp"
    #include"memory/new.hpp"
    #include"macro/xdebug.hpp"
    #include"macro/xdebug_fail.hpp"
    #include"instruction/bit_test.hpp"
    #include"instruction/bit_test_and_set.hpp"
    #include"instruction/bit_test_and_reset.hpp"

    namespace xuser{
        extern voidp malloc(size_t bytes);
        extern voidp malloc_aligned(size_t bytes, size_t align_bytes);
        extern void  free(voidp ptr);
        extern void  free_aligned(voidp ptr);

        typedef struct node{
            node * previous;
            node * next;
        } * nodep;

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

            // 用于获取页内偏移
            mask_to_get_offset      = page_bytes - 1,

            // 用于获取页首地址
            mask_to_get_header      = ~mask_to_get_offset,

            // 页首部位指示器位数
            max_bits                = page_bytes / scale_one,
        };

        typedef struct page_header{
        private:
            u08 bmp[max_bits / 8] = {0};
            struct pair{ nodep begin = nullptr; uxx length = 0; };
        public:
            page_header();

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

            pair left_free_block_of(nodep cur){
                if (uxx index = index_of(cur); get(index - 1)){
                    return pair{};
                }
                else if (get(index - 2)){
                    return pair{ cur - 1, 1 };
                }
                else{
                    auto len = *uxxp(cur - 1);
                    return pair{ cur - len ,len };
                }
            }

            pair right_free_block_of(nodep cur, uxx length){
                if ((mask_to_get_offset & uxx(cur += length)) == 0){
                    return {};
                }
                if (uxx index = index_of(cur); get(index)){
                    return {};
                }
                else if (get(index + 1)){
                    return { cur, 1 };
                }
                else{
                    return { cur, node_plusp(cur)->blocks };
                }
            }

            nodep first_block(){
                return nodep(this + 1);
            }

            void set_rest(nodep cur, uxx length){
                if (length <= 1){
                    return;
                }
                (node_plusp(cur))->blocks   = length;
                (uxxp(cur + length - 1))[0] = length;
            }

            void mark_in_use(nodep cur, uxx length){
                uxx index = index_of(cur);
                set(index);
                set(index + length - 1);
            }

            void mark_free(nodep cur, uxx length){
                uxx index = index_of(cur);
                reset(index);
                reset(index + length - 1);
            }

            page_header * previous;
            page_header * next;
        } * page_headerp;

        enum : uxx{
            // 一个页一共有多少可用块
            page_block_count        = (page_bytes - sizeof(page_header)) / scale_one,
        };

        inline page_header::page_header(){
            // 设置哨兵位
            set(uxx(-1));
            set(page_block_count);
        }

        // 必须是 2 的次幂
        static_assert((page_bytes & (page_bytes - 1)) == 0);
    }

    namespace xuser::origin{
        struct tiny_allocator{
        private:
            using indicator_t = inc::bit_indicator<scale>;
        public:
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
                pused_bytes         += bytes;
                pneed_free_count    += 1;

                auto require_size_index = (bytes - 1) / scale_one;
                auto miss = false;

                // slot 中置位位表示空闲的块
                // 选择最接近但不小于所需大小的块
                if (require_size_index < boundary){
                    if (auto closest_index = slot.index_of_first_set(require_size_index); closest_index != not_exist){
                        return take_out(closest_index, require_size_index, slot, free_list_array);
                    }
                    else{
                        miss = true;
                    }
                }
                if (require_size_index < page_block_count){
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

                pused_bytes           -= bytes;
                pneed_free_count      -= 1;

                auto return_size_index = (bytes - 1) / scale_one;

                if (return_size_index >= page_block_count){
                    palive_pages       -= 1;
                    xuser::free(ptr);
                    return;
                }

                auto   cur             = nodep(ptr);
                auto & bmp             = get_page_header_by(cur);
                auto   block_index     = bmp.index_of(cur);

                xdebug_fail(bmp.get(page_block_count) == 0){
                    xdebug(im_memory_tiny_allocator_free, block_index, bmp.get(block_index), ptr, bytes, "maybe repeated release");
                    return;
                }
                xdebug_fail(bmp.get(block_index) == 0){
                    xdebug(im_memory_tiny_allocator_free, block_index, bmp.get(block_index), ptr, bytes, "maybe repeated release");
                    return;
                }

                auto free_block = cur;
                auto free_size  = return_size_index + 1;
                auto left       = bmp.left_free_block_of(cur);
                auto right      = bmp.right_free_block_of(cur, free_size);

                if (bmp.mark_free(cur, return_size_index + 1); left.begin != nullptr){
                    bmp.mark_free(left.begin, left.length);
                    free_block  = left.begin;
                    free_size  += left.length;
                    remove(left.begin, left.length);
                }
                if (right.begin != nullptr){
                    free_size  += right.length;
                    bmp.mark_free(right.begin, right.length);
                    remove(right.begin, right.length);
                }

                // 整个页都空闲时就释放该页
                if (free_size == page_block_count){
                    origin_free(xref bmp);
                    return;
                }

                // 否则合成的更大的空闲块放回对应的链表数组中
                append(free_block, free_size);
            }

        private:
            void origin_free(page_header * ptr){
                auto next      = ptr->next;
                auto prev      = ptr->previous;
                xuser::free_aligned(ptr);
                palive_pages   -= 1;

                if (next == ptr){
                    page_list  = nullptr;
                    return;
                }
                if (ptr == page_list){
                    page_list = next;
                }

                next->previous = prev;
                prev->next     = next;
            }

            voidp origin_alloc(uxx require_size_index){
                // 超出管理范畴
                if (palive_pages += 1; require_size_index > page_block_count){
                    return xuser::malloc((require_size_index + 1) * scale_one);
                }

                auto meta   = xuser::malloc_aligned(page_bytes, page_bytes);
                auto page   = new(meta) page_header;
                auto first  = page->first_block();

                if (page_list == nullptr){
                    page_list               = page;
                    page->next              = page->previous = page;
                }
                else{
                    auto first              = page_list;
                    first->previous->next   = page;
                    page->previous          = first->previous;
                    first->previous         = page;
                    page->next              = first;
                }
                return split(first, page_block_count, require_size_index);
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

            voidp split(nodep header, uxx total_size, uxx require_size_index){
                if (get_page_header_by(header).mark_in_use(header, require_size_index + 1);
                    total_size > require_size_index + 1){
                    auto require_size    = require_size_index + 1;
                    auto rest            = header + require_size;
                    auto rest_size       = total_size - require_size;
                    auto rest_size_index = rest_size - 1;
                    xdebug(im_memory_tiny_allocator_split, header, rest, total_size, require_size, rest_size);
                    xdebug_fail(rest_size > total_size);
                    append(rest, rest_size);
                }
                return header;
            }

            void append(node * block, uxx index, indicator_t & slot, node ** free_list_array){
                xdebug(im_memory_tiny_allocator_append, index, slot.get(index));

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
                xdebug(im_memory_tiny_allocator_remove, block, next, index);

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

            indicator_t     slot;
            indicator_t     slot_plus;
            uxx             palive_pages         = 0;
            uxx             pused_bytes         = 0;
            uxx             pneed_free_count    = 0;
            page_header *   page_list           = nullptr;
            node        *   free_list_array[scale];
            node        *   free_list_array_plus[scale];
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::memory_private_tiny_allocator::origin;
}