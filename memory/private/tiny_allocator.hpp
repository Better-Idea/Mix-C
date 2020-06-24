#ifndef xpack_memory_private_tiny_allocator
#define xpack_memory_private_tiny_allocator 
    #pragma push_macro("xuser")  
    #undef  xuser
    #define xuser mixc::memory_private_tiny_allocator
    #include"define/base_type.hpp"
    #include"docker/bit_indicator.hpp"
    #include"memory/new.hpp"
    #include<malloc.h>

    namespace xuser::origin{
        typedef struct node{
            node * previous;
            node * next;
        } * nodep;

        enum : uxx{
            min_base_block_size     = 16,
            max_base_block_size     = 1024,
            max_base_block_count    = max_base_block_size / min_base_block_size,
            page_size               = 64 * max_base_block_size,
            mask_to_get_offset      = page_size - 1,
            mask_to_get_header      = ~mask_to_get_offset,
            max_bits                = page_size / min_base_block_size,
        };

        typedef struct page_header : inc::bit_indicator<max_bits>{
            page_header * previous;
            page_header * next;
        } * page_headerp;

        enum : uxx{
            index_of_top_block      = (page_size / min_base_block_size - 1),
            index_of_bottom_block   = (sizeof(page_header) + min_base_block_size - 1) / min_base_block_size,
            max_count_of_block      = (page_size - sizeof(page_header)) / min_base_block_size,
        };

        struct classifier{
        private:
            enum{
                max_slot_size = 64,
            };
            using indicator_t = inc::bit_indicator<max_slot_size>;
        public:
            voidp alloc(uxx require_size_index){
                // slot 中置位位表示空闲的块
                // 选择最接近但不小于所需大小的块
                if (require_size_index < max_base_block_count){
                    if (auto closest_index = slot.index_of_first_set(require_size_index); closest_index != not_exist){
                        return take_out(closest_index, require_size_index, slot, free_list_array);
                    }
                }
                if (require_size_index < max_count_of_block){
                    if (auto closest_index = slot_plus.index_of_first_set(require_size_index / max_base_block_count); closest_index != not_exist){
                        return take_out(closest_index, require_size_index, slot_plus, free_list_array_plus);
                    }
                }
                return origin_alloc(require_size_index);
            }

            void free(voidp ptr, uxx return_size){
                if (return_size >= max_count_of_block){
                    ::free(ptr);
                    return;
                }

                auto   block = nodep(ptr);
                auto & idc   = get_page_header_by(block);
                auto   base  = nodep(& idc);
                auto   begin = index_of(block);
                auto   left  = begin;
                auto   right = begin + return_size;
                uxx    count;

                idc.reset(left).reset(right);

                // 若左边相邻块空闲
                if (begin != index_of_bottom_block and idc.get(begin - 1) == 0){
                    left    = idc.index_of_last_set(begin - 1) + 1;
                    block   = & base[left]; 
                    remove(block, begin - left - 1);
                }

                // 若右边相邻块空闲
                if (begin != index_of_top_block and idc.get(begin + 1) == 0){
                    right   = idc.index_of_first_set(begin + 1);
                    right   = right == not_exist ? index_of_top_block : right - 1;
                    remove(& base[right], right - begin);
                }

                if (count = right - left + 1; count == max_count_of_block){
                    origin_free(page_headerp(base));
                    return;
                }

                if (count > max_base_block_count){
                    append(block, count - 1, slot_plus, free_list_array_plus);
                }
                else{
                    append(block, count - 1, slot, free_list_array);
                }
            }
        private:
            void origin_free(page_header * ptr){
                auto temp = ptr;
                auto next = ptr->next;

                if (next == temp){
                    page_list = nullptr;
                    return;
                }
                if (ptr == page_list){
                    page_list = next;
                }

                next->previous = temp->previous;
                temp->previous->next = next;
            }

            voidp origin_alloc(uxx require_size_index){
                // 超出管理范畴
                if (require_size_index > max_count_of_block){
                    return malloc((require_size_index + 1) * min_base_block_size);
                }

                auto meta = _mm_malloc(page_size, page_size);
                auto page = new(meta) page_header;

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

                auto base      = nodep(page);
                auto first     = & base[index_of_bottom_block];
                return split(first, max_count_of_block, require_size_index);
            }

            voidp take_out(uxx closest_index, uxx require_size_index, indicator_t & slot, node ** free_list_array){
                auto first  = free_list_array[closest_index];
                auto next   = first->next;
                
                // 若当前元素时该链表中最后一个元素，则将该位置的 slot 设置为空
                if (next == first){
                    slot.reset(closest_index);
                }
                // 否则让下一个元素顶替 first 的位置
                else{
                    next->previous                  = first->previous;
                    first->previous->next           = next;
                    free_list_array[closest_index]  = next;
                }

                auto & idc          = get_page_header_by(first);
                auto   begin        = index_of(first);
                auto   end          = idc.index_of_first_set(begin + 1);

                if (end == not_exist){
                    end = index_of_top_block;
                }

                // 在 first 块所在的页中标记该块所占用的范围，以便为块合并提供指引
                idc.set(begin).set(begin + require_size_index);

                auto   total_size   = end - begin + 1;
                return split(first, total_size, require_size_index);
            }

            voidp split(nodep first, uxx total_size, uxx require_size_index){
                // 从 first 开始的地址
                // [0, require_size_index] 属于要分配出去的块
                // (require_size_index, real_size) 属于结余的块
                // 将结余部分返回对应的 slot
                auto   require_size = require_size_index + 1;
                auto   rest         = first + require_size;
                auto   rest_size    = total_size - require_size;

                if (rest_size > max_base_block_count){
                    append(rest, rest_size * min_base_block_size / max_base_block_size - 1, slot_plus, free_list_array_plus);
                }
                else if (rest_size != 0){
                    append(rest, rest_size - 1, slot, free_list_array);
                }
                return first;
            }

            void append(node * block, uxx index, indicator_t & slot, node ** free_list_array){
                if (slot.get(index) == 0){
                    slot.set(index);
                    free_list_array[index]  = block;
                }
                else{
                    auto first              = free_list_array[index];
                    block->previous         = first->previous;
                    block->next             = first;
                    first->previous->next   = block;
                    first->previous         = block;
                }
            }

            void remove(node * block, uxx size){
                auto   is_plus      = size >= max_base_block_count;
                auto   next         = block->next;
                auto & free_list    = is_plus ? 
                    this->free_list_array_plus[size >>= max_base_block_count] : 
                    this->free_list_array[size];
                auto & slot         = is_plus ?
                    this->slot_plus :
                    this->slot;

                if (next == block){
                    slot.reset(size);
                    return;
                }
                if (block == free_list){
                    free_list = next;
                }

                block->previous->next = next;
                next->previous = block->previous;
            }

            page_header & get_page_header_by(node * ptr){
                // 因为在 ptr 块都是来自 64KB 地址对齐的页，所以可以通过屏蔽低 16bit 来得到页首地址
                auto begin = uxx(ptr) & mask_to_get_header;
                return *((page_header *)begin);
            }

            uxx index_of(node * ptr){
                // 页内分配的最小单位为 16Byte 的块
                auto offset = uxx(ptr) & mask_to_get_offset;
                return offset / min_base_block_size;
            }

            indicator_t     slot;
            indicator_t     slot_plus;
            page_header *   page_list;
            node        *   free_list_array[max_slot_size];
            node        *   free_list_array_plus[max_slot_size];

        };
    }
    #pragma pop_macro("xuser")

    namespace xuser::inc{
        using namespace ::mixc::memory_private_tiny_allocator::origin;
    }
#endif
