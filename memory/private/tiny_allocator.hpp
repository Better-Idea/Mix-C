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
    #include<malloc.h>

    namespace xuser{
        typedef struct node{
            node * previous;
            node * next;
        } * nodep;

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
            // 在此对 bit_indicator 包装一下
            // 在外部间接访问 idc 时
            // 对外使用下标 [1, max_bits] 实际对应 idc 的 [0, max_bits - 1]
            inc::bit_indicator<max_bits> idc;
        public:
            page_header();

            uxx index_of_first_set(uxx begin){
                return idc.index_of_first_set(begin - 1) + 1;
            }
            uxx index_of_last_set(uxx end){
                return idc.index_of_last_set(end - 1) + 1;
            }
            bool get(uxx index){
                return idc.get(index - 1);
            }
            void set(uxx index){
                idc.set(index - 1);
            }
            void reset(uxx index){
                idc.reset(index - 1);
            }
            page_header * previous;
            page_header * next;
        } * page_headerp;

        enum : uxx{
            // 每一个页都包含 page_header 首部，首部之后的内存是可以分配的
            // 页第一个可用块的索引
            index_of_bottom_block   = (sizeof(page_header) + scale_one - 1) / scale_one,

            // 页最后一个可以块的索引
            index_of_top_block      = (page_bytes / scale_one - 1),

            // 一个页一共有多少可用块
            page_block_count        = (page_bytes - sizeof(page_header)) / scale_one,
        };

        inline page_header::page_header(){
            // 设置哨兵位
            set(index_of_bottom_block - 1);
            set(index_of_top_block + 1);
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

            voidp alloc(uxx bytes){
                pused_bytes         += bytes;
                pneed_free_count    += 1;

                auto require_size_index = (bytes - 1) / scale_one;

                // slot 中置位位表示空闲的块
                // 选择最接近但不小于所需大小的块
                if (require_size_index < boundary){
                    if (auto closest_index = slot.index_of_first_set(require_size_index); closest_index != not_exist){
                        return take_out(closest_index, require_size_index, slot, free_list_array);
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
                pused_bytes         -= bytes;
                pneed_free_count    -= 1;

                xdebug_fail(bytes == 0){
                    return;
                }

                auto return_size_index = (bytes - 1) / scale_one;

                if (return_size_index >= page_block_count){
                    ::free(ptr);
                    return;
                }

                auto   block = nodep(ptr);
                auto & idc   = get_page_header_by(block);
                auto   base  = nodep(& idc);
                auto   begin = index_of(block);
                auto   left  = begin;
                auto   right = begin + return_size_index;
                uxx    count;

                xdebug_fail(left < index_of_bottom_block){
                    xdebug(im_memory_classifier_free, left, index_of_bottom_block, ptr, bytes, "unexcept release address");
                    return;
                }
                xdebug_fail(idc.get(left) == 0){
                    xdebug(im_memory_classifier_free, left, idc.get(begin), ptr, bytes, "maybe repeated release");
                    return;
                }

                auto except_right = idc.index_of_first_set(begin + 1);
                xdebug_fail(right > except_right){
                    xdebug(im_memory_classifier_free, left, right, except_right, ptr, bytes, "unexcept release bytes");
                    return;
                }

                idc.reset(left);
                idc.reset(right);

                // 若左边相邻块空闲
                // 因为左、右边界都设置了哨兵位，所以 index_of_xxxx_set 返回值不会是 not_exist
                if (idc.get(left - 1) == 0){
                    left    = idc.index_of_last_set(left - 1) + 1;
                    remove(& base[left], begin - left);
                }

                // 若右边相邻块空闲
                if (idc.get(right + 1) == 0){
                    right   = idc.index_of_first_set(right + 1) - 1;
                    remove(& base[begin + return_size_index + 1], right - begin);
                }

                // 整个页都空闲时就释放该页
                if (count = right - left + 1; count == page_block_count){
                    origin_free(page_headerp(base));
                    return;
                }

                // 否则合成的更大的空闲块放回对应的链表数组中
                if (count > boundary){
                    append(block, count / boundary - 1, slot_plus, free_list_array_plus);
                }
                else{
                    append(block, count - 1, slot, free_list_array);
                }
            }

        private:
            void origin_free(page_header * ptr){
                auto next = ptr->next;
                auto prev = ptr->previous;
                ::_mm_free(ptr);

                if (next == ptr){
                    page_list = nullptr;
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
                if (require_size_index > page_block_count){
                    return malloc((require_size_index + 1) * scale_one);
                }

                auto meta   = _mm_malloc(page_bytes, page_bytes);
                auto page   = new(meta) page_header;
                page->set(index_of_bottom_block);
                page->set(index_of_bottom_block + require_size_index);

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

                auto base   = nodep(page);
                auto first  = & base[index_of_bottom_block];
                return split(first, page_block_count, require_size_index);
            }

            voidp take_out(uxx closest_index, uxx require_size_index, indicator_t & slot, node ** free_list_array){
                auto first = free_list_array[closest_index];
                auto next  = first->next;
                
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

                // 在 first 块所在的页中标记该块所占用的范围，以便为块合并提供指引
                idc.set(begin);
                idc.set(begin + require_size_index);
                xdebug(im_memory_classifier_take_out, 
                    begin, 
                    idc.get(begin),
                    begin + require_size_index,
                    idc.get(begin + require_size_index)
                );

                auto   total_size   = end - begin;
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

                xdebug(im_memory_classifier_split, first, rest, total_size, require_size, rest_size);

                if (rest_size > boundary){
                    append(rest, rest_size * scale_one / scale_two - 1, slot_plus, free_list_array_plus);
                }
                else if (rest_size != 0){
                    append(rest, rest_size - 1, slot, free_list_array);
                }
                return first;
            }

            void append(node * block, uxx index, indicator_t & slot, node ** free_list_array){
                xdebug(im_memory_classifier_append, index, slot.get(index));

                if (slot.get(index) == 0){
                    slot.set(index);
                    free_list_array[index]  = block;
                    block->next             = block;
                    block->previous         = block;
                }
                else{
                    auto first              = free_list_array[index];
                    block->previous         = first->previous;
                    block->next             = first;
                    first->previous->next   = block;
                    first->previous         = block;
                }
            }

            void remove(node * block, uxx block_size){
                auto   block_size_index     = block_size - 1;
                auto   is_plus              = block_size_index >= boundary;
                auto   next                 = block->next;
                auto & free_list            = is_plus ? 
                    this->free_list_array_plus[block_size_index = block_size / boundary - 1] : 
                    this->free_list_array[block_size_index];
                auto & slot                 = is_plus ?
                    this->slot_plus :
                    this->slot;

                if (next == block){
                    slot.reset(block_size_index);
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
                return offset / scale_one;
            }

            indicator_t     slot;
            indicator_t     slot_plus;
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