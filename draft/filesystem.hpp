#ifndef xpack_draft_file_system
#define xpack_draft_file_system
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_file_system::inc
#include"interface/can_callback.hpp"
#include"lang/cxx/is_starts_with.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx.hpp"
#include"instruction/index_of_last_set.hpp"
#include"memop/copy.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::draft_file_system{
    constexpr uxx page_size = 512;

    enum default_map_t{
        page_of_root_dir                = 4,
        page_of_allocator               = 5,
        page_of_entry                   = 6,
        page_of_file_system_header      = 7,
        page_of_linear_begin            = 8,
    };

    struct page_group{
        /* 512 512 1K 2K
         * 4K 8K 16K 32K
         */
        u32                 page_lv[7]              = {0};
        u32                 page_table              =  0;
    };

    struct file_header{
        u32                 bytes;
        u32                 create_time;
        u32                 modify_time;
        u32                 r0              : 27;
        u32                 write_times     : 5;
        page_group          content;
        u08                 r1[15];
        u08                 name_length             = 0;
        u08                 name[64];
    };

    struct direcotry_header{
        u32                 create_time;
        u32                 r0              : 25    = 0;
        u32                 write_times     : 6     = 0;
        u08                 r1[7];
        u08                 name_length;
        u08                 name[40];

        enum{ max_write_times = 63 };
        struct subset{
            u16             count                   = 0;
            u16             redirection_count       = 0;
            page_group      header;
        } file, subdir;
    };

    enum{
        k = sizeof(file_header),
        s = sizeof(direcotry_header)
    };

    struct free_page{
        u32                 next                    = 0;
        u32                 items[page_size / sizeof(u32) - 1];
    };

    struct page_allocator{
        u64                 timestamp               = 0;
        u32                                 : 24;
        u32                 write_times     : 8     = 0;

        struct{
            u32             begin                   = page_of_linear_begin;
            u32             end                     = 0;
            u32             current                 = page_of_linear_begin;
        } linear_page;

        /*  256     512     1K      2K
         *  4K      8K      16K     32K
         *  64K
         */
        u32                 free_page_lv[9]         = {0};
    };

    struct entry_page{
        u64                 timestamp;
        u32                                 : 24;
        u32                 write_times     : 8     = 0;
        u32                 page_allocator          = page_of_allocator;
        u32                 root_dir                = page_of_root_dir;
    };

    struct file_system_header{
        u64                 r0                      = 0;
        u08                 token[16]               = "Mix-C FS";
        u08                 name[64];
        u64                 capacity;   // 容量
        u08                 serial;     // 系列
        u08                 major;      // 大改进
        u08                 stepping;   // 新增功能
        u08                 revision;   // 优化
        u64                 entry_page              = page_of_entry;
    };

    constexpr inc::c08 directory_already_exist      = "directory already exist";

    constexpr inc::c08 capacity_too_small           = "capacity too small";
    constexpr inc::c08 name_too_long                = "name too long";
    constexpr inc::c08 success                      = "";

    constexpr uxx page_of_null = 0;

    struct file_system{
        using access_invoke = inc::can_callback<void(u64 address, voidp buffer, u64 bytes)>;

        access_invoke reader;
        access_invoke writer;

        auto create_system(u64 capacity, inc::c08 disk_name = "Mix-C Disk"){
            auto && header          = file_system_header();
            auto && entry           = entry_page();
            auto && allocator       = page_allocator();
            auto && root_dir        = direcotry_header();
            auto && linear          = allocator.linear_page;
            auto    len             = disk_name.length(); 
            auto    pages           = bytes_to_page(capacity);
            header.capacity         = capacity;
            linear.end              = pages - 1;
            root_dir.create_time    = now();

            if (header.capacity <= 1024 * 1024){
                return capacity_too_small;
            }
            if (len > sizeof(header.name)){
                return name_too_long;
            }
            if (len != sizeof(header.name)){
                header.name[len] = '\0';
            }

            inc::copy_with_operator(header.name, disk_name, len);

            write_block(page_of_root_dir, xref root_dir, sizeof(root_dir));
            write_block(page_of_allocator, xref allocator, sizeof(allocator));
            write_block(page_of_entry, xref entry, sizeof(entry));
            write_block(page_of_file_system_header, xref header, sizeof(header));
            return success;
        }

        auto create_dir(direcotry_header & parent, direcotry_header * buffer, inc::c08 name){
            struct{
                u32  i;
                u32  i_lv;
                u32  i_page;
                u32  i_page_offset;
                bool is_exist           = false;

                enum{
                    items_per_page      = (page_size / sizeof(direcotry_header))
                };

                void normalize(u32 index){
                    uxx i_msb;
                    i                   = (index % items_per_page);
                    i_page              = (index / items_per_page);
                    i_msb               = (inc::index_of_last_set(i_page));
                    i_lv                = (i_msb + 1);
                    i_page_offset       = (i_page & ((1 << i_msb) - 1));
                }
            } r;

            auto &  subdir              = (parent.subdir);
            auto &  header              = (subdir.header);
            auto && new_dir             = (direcotry_header{});
            auto    item                = (buffer);
            auto    index               = (u32)0;
            auto    page                = (u32)0;
            auto    total_count         = (subdir.count + subdir.redirection_count);
            
            while(index < total_count){
                if (r.normalize(index); r.i == 0){
                    page                = (header.page_lv[r.i_lv]);
                    read_block(page + r.i_page_offset, buffer, page_size);
                }
                if (item[r.i].write_times == direcotry_header::max_write_times){
                    index              += 1;
                    continue;
                }
                if (inc::c08 token{item[r.i].name, item[r.i].name_length}; token.compare_fastly(name) == 0){
                    r.is_exist          = true;
                    break;
                }
            }
            return r;
        }

        void create_dir(){
            
        }

        u32 alloc_page(::length count){

        }

        auto create_file(inc::c08 const * path, uxx length){

        }

        void write_block(u32 page, voidp buffer, u64 bytes){
            writer(page * page_size, buffer, bytes);
        }

        void write_byte(u64 address, voidp buffer, u64 bytes){
            writer(address, buffer, bytes);
        }

        void write_byte(u64 * address, voidp buffer, u64 bytes){
            writer(*address, buffer, bytes);
            *address += bytes;
        }

        void read_block(u32 page, voidp buffer, u64 bytes){
            reader(page * page_size, buffer, bytes);
        }

        void read_byte(u64 address, voidp buffer, u64 bytes){
            reader(address, buffer, bytes);
        }

        void read_byte(u64 * address, voidp buffer, u64 bytes){
            reader(*address, buffer, bytes);
            *address += bytes;
        }

        static u32 bytes_to_page(u64 bytes){

        }

        static u32 now(){

        }
    };

}

#endif
