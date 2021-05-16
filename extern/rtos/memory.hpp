#ifndef xpack_extern_rtos_memory
#define xpack_extern_rtos_memory 
#pragma push_macro("xuser")  
#undef  xuser
#define xuser mixc::extern_rtos_memory::inc
#include"define/base_type.hpp"
#include"instruction/bit_test.hpp"
#include"instruction/bit_test_and_set.hpp"
#include"instruction/bit_test_and_reset.hpp"
#include"instruction/index_of_first_set.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"memop/addressof.hpp"
#include"memop/zeros.hpp"
#include"meta/has_constructor.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_rtos_memory{
    struct page_header;

    typedef struct node{
        node      * prev;
        node      * next;

        page_header * offset_to_header(){
            auto begin = uxx(this) & ~mask_to_get_header;
            return (page_header *)begin;
        }
    } * nodep;

    typedef struct node_plus : node{
        uxx         blocks;
        uxx         padding;
    } * node_plusp;

    enum : uxx{
        shift_to_get_scale_one  = 6,
        scale_one               = 1 << shift_to_get_scale_one,
        scale_two               = scale_one * scale_one,

        scale_level             = 2,

        // 小块以 size_one 为区分单位
        // 无论是大块还是小块，size_one 都是分配的最小单位
        size_one                = 16,

        // 大块以 size_two 为区分单位
        size_two                = size_one * scale_one,

        // 大块以 size_two 为区分单位
        size_three              = size_two * scale_one,

        mask_to_get_header      = (1 << 16) - 1,
    };

    
    typedef struct page_header{
    private:
        u16           bmp_bytes;
        u16           blocks;
        struct pair{ nodep begin = nullptr; uxx length = 0; };

        u08p bmp_area(){
            return u08p(this + 1);
        }

        nodep data_area(){
            return nodep(u08p(this + 1) + bmp_bytes);
        }

    public:
        page_header *  prev;
        page_header *  next;
        page_header(uxx bytes){
            bmp_bytes = bytes / size_one / 8;
            auto cost = sizeof(page_header) + bmp_bytes;
            cost     += (size_one - 1);
            cost     &= ~(size_one - 1);
            blocks    = bytes - cost;

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
            inc::bit_test_and_set(xref bmp_area()[index >> 3], index & 0x7);
        }

        // 标记空闲
        void reset(uxx index){
            index += 1;
            inc::bit_test_and_reset(xref bmp_area()[index >> 3], index & 0x7);
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
    } * page_headerp;

    struct slot_t{
        uxx index_of_closest(uxx i_expect){
            u64 mask = ~((u64(1) << i_expect) - 1);
            u64 masked = slot & mask;
            return inc::index_of_first_set(masked);
        }

        bool test_and_reset(uxx index){
            return inc::bit_test_and_reset(xref slot, index);
        }

        bool test_and_set(uxx index){
            return inc::bit_test_and_set(xref slot, index);
        }
    private:
        u64 slot = 0;
    };

    struct tiny_allocator_header{
        slot_t          slot[scale_level]       = {};
        uxx             pused_bytes             = 0;
        uxx             palive_object           = 0;
        uxx             max_block_bytes         = 0;
        page_header   * page_list               = nullptr;
        node          * free_list_array[scale_level][scale_one];
    };
}

namespace mixc::extern_rtos_memory::origin{
    struct tiny_allocator : tiny_allocator_header{
        uxx used_bytes() {
            return pused_bytes;
        }

        uxx alive_object() {
            return palive_object;
        }

        template<class item_t, class ... args_t>
        requires(inc::has_constructor<item_t, void(args_t...)>)
        item_t * alloc(args_t const & ... list){
            auto ptr = this->alloc(sizeof(item_t));
            if (ptr == nullptr){
                return ptr;
            }
            return xnew(ptr) item_t(list...);
        }

        voidp alloc(uxx bytes){
            auto i_expect           = (bytes - 1) / size_one;
            auto i_index            = (i_expect);
            auto expect_blocks      = (i_expect + 1);

            // slot 中置位位表示空闲的块
            // 选择最接近但不小于所需大小的块
            for(uxx i = 0; i < scale_level; i++){
                if (auto i_closest = slot[i].index_of_closest(i_index); i_closest != not_exist){
                    pused_bytes    += (bytes);
                    palive_object  += (1);
                    return take_out(i_closest, i_index, expect_blocks, slot[i], free_list_array[i]);
                }
                if (i_index){
                    i_index         = (i_index - 1) / scale_one;
                }
            }
            return nullptr;
        }

        template<class item_t>
        void free(item_t const * ptr){
            if (ptr == nullptr){
                return;
            }

            ptr->~item_t();
            this->free(ptr, sizeof(item_t));
        }

        void free(voidp block, uxx bytes){
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
            this->append(page, ptr, free_blocks);
        }

        bstate_t mount(voidp ptr, uxx bytes){
            // 地址没有对齐
            if (uxx(ptr) & mask_to_get_header){
                return bstate_t::fail;
            }

            auto page = xnew(ptr) page_header(bytes); 

            if (page_list == nullptr){
                page_list           = page;
                page->next          = page->prev = page;
            }
            else{
                auto prev           = page_list->prev;
                prev->next          = page;
                page->prev          = prev;
                page_list->prev     = page;
                page->next          = page_list;
            }

            this->append(page, page->first_block(), page->total_blocks());
            return bstate_t::success;
        }

    private:
        void append(page_header * page, nodep ptr, uxx free_blocks){
            for(uxx i = 0, i_expect = free_blocks - 1; i < scale_level; i++){
                if (i_expect < scale_one){
                    this->append(ptr, i_expect, slot[i], free_list_array[i]);
                    break;
                }
                if (i_expect){
                    i_expect        = (i_expect - 1) / scale_one;
                }
            }

            page->set_rest(ptr, free_blocks); 
        }

        voidp take_out(uxx i_closest, uxx i_expect, uxx expect_blocks, slot_t & slot, node ** free_list){
            auto first              = free_list[i_closest];
            auto page               = first->offset_to_header();
            auto next               = first->next;
            auto real_blocks        = 
                free_list == free_list_array[0] ? 
                i_closest + 1 :
                node_plusp(first)->blocks;

            // 若当前元素时该链表中最后一个元素，则将该位置的 slot 设置为空
            if (next == first){
                slot.test_and_reset(i_closest);
            }
            // 否则让下一个元素顶替 first 的位置
            else{
                next->prev          = first->prev;
                first->prev->next   = next;
                free_list[i_closest]= next;
            }

            // first 作为分配出去的块，rest 作为剩下的块放回
            if (page->mark_in_use(first, expect_blocks); real_blocks > expect_blocks){
                auto rest           = xref first[expect_blocks];
                auto rest_blocks    = real_blocks - expect_blocks;
                this->append(page, rest, rest_blocks);
            }
            return first;
        }

        void append(node * ptr, uxx i_expect, slot_t & slot, node ** free_list_array){
            if (slot.test_and_set(i_expect)){
                auto header         = free_list_array[i_expect];
                ptr->prev           = header->prev;
                ptr->next           = header;
                header->prev->next  = ptr;
                header->prev        = ptr;
            }
            else{
                free_list_array[i_expect]  = ptr;
                ptr->next           = ptr;
                ptr->prev           = ptr;
            }
        }

        void remove(node * ptr, uxx i_expect, slot_t & slot, node ** free_list){
            auto next               = ptr->next;
            auto prev               = ptr->prev;

            if (next == ptr){
                slot.test_and_reset(i_expect);
                return;
            }
            if (ptr == free_list[i_expect]){
                free_list[i_expect] = next;
            }

            next->prev              = prev;
            prev->next              = next;
        }

        void remove(node * ptr, uxx blocks){
            for(uxx i = 0, i_expect = blocks - 1; i < scale_level; i++){
                if (i_expect < scale_one){
                    this->remove(ptr, i_expect, slot[i], free_list_array[i]);
                    break;
                }
                if (i_expect){
                    i_expect        = (i_expect - 1) / scale_one;
                }
            }
        }
    };
}

#endif

xexport_space(mixc::extern_rtos_memory::origin)
