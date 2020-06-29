#ifndef xpack_extern_mmu_x3240
#define xpack_extern_mmu_x3240
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_mmu_x3240
        #include"define/base_type.hpp"
        #include"docker/bit_indicator.hpp"
        #include"instruction/count_of_set.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_mmu_x3240::origin{
        struct object_group_header{
            u32 previous;
            u32 next;

            // 用于在访问对象时检查所有权
            u64 owner_id    : 14;
            u64 owner_subid : 14;

            u64             : 4;

            // 物理地址 = segment << 16 | object_descriptor.offset << 4
            u64 segment     : 24;

            // 当前 object_group 空闲 object_descriptor 个数
            u64 free_count  : 8;
        };

        struct page64k_header{
            u32 group_header;
            u32 recycle_items_list;
            u32 linear_items;
        };

        struct object_descriptor_tiny{
            u32 valid               : 1;
            u32 readonly            : 1;
            u32 executable          : 1;
        private:
            u32 tiny                : 1;
        public:
            u32                     : 4;
            u32 data                : 24;

            object_descriptor_tiny(){
                *u32p(this) = 0;
                tiny        = true;
            }
        };

        struct object_descriptor{
            u32 valid               : 1;
            u32 readonly            : 1;
            u32 executable          : 1;
        private:
            u32 tiny                : 1;
        public:
            u32 bit_header          : 1;        // 代替 free_block_header.is_free 位
            u32 bit_end             : 1;        // 代替 free_block_end.is_free 位
            u32                     : 6;
            u32 pageing             : 1;
            u32 size                : 7;        // size * 16 byte
            u32 offset              : 12;       // offset * 16 -> 16bit

            object_descriptor(){
                *u32p(this) = 0;
                tiny        = false;
            }
        };

        /*
         * +----------------+
         * |  block layout  |
         * |---------------------------------------------------------+
         * | is_free | previous | next | size | ... | size | is_free |
         * +---------------------------------------------------------+
         * lsb                                                     msb
         * 
         * 当空闲块只有 16 字节时， lsb 侧的 size 和 msb 侧的 size 重合
         */
        struct free_block_header{
            // 该位用于指示从该位所在的块是否空闲，该位不做数据位
            // 该位实际的数据位在 object_descriptor.bit_header
            u64 is_free             : 1;

            // 当前块不是空闲时，is_free 之后的位作为数据内存使用
            u64                     : 3;
            u64 previous            : 40;
            u64                     : 20;
            u64 next                : 40;
            u64 size                : 20;
            u64                     : 4;
        };

        struct free_block_end{
            // 当前块不是空闲时，is_free 之前的位作为数据内存使用
            u64                     : 40;
            u64 size                : 20;
            u64                     : 3;

            // 该位用于指示该位所在的块是否空闲，该位不做数据位
            // 该位实际的数据位在 object_descriptor.bit_end
            u64 is_free             : 1;
        };


        struct page_header{
            u32 owner;
        };

        struct page_table{
            struct pair{
                u32 replace_field;
                u32 page;
            };

            // - 0：固定大小模式
            // - 1：动态增长模式
            u32 mode                : 1;
            u32                     : 3;
            u32 size                : 28;

            pair & operator[](uxx index){
                return ((pair *)(this + 1))[index];
            }

            uxx offset_to_big_page(){
                auto small = size & ~(uxx(-1) << (28 - 4));
                auto count = inc::count_of_set(small);
                return count;
            }

            u32 & big_page(uxx index){
                auto ofs = offset_to_big_page();
                auto ptr = u32p(& this[0][ofs]);
                return ptr[index];
            }

            uxx big_page_count(){
                return size >> (24 - 4);
            }
        };

        struct object_group{
        private:
            enum{ for_header = 4 };
        public:
            object_group_header header;
            object_descriptor   items[256 - for_header];
            static_assert(sizeof(object_group_header) == sizeof(object_descriptor) * for_header);
        };

        struct sysreg{
            inc::bit_indicator<128> is_used;
            inc::bit_indicator<128> is_objs;

            union{
                u08p m08;
                u16p m16;
                u32p m32;
                u64p m64;
            };

            u64 total_bytes;
        };

        struct vcpu{

        };
    }
#endif

