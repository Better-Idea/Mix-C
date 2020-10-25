#ifndef xpack_draft_file_system
#define xpack_draft_file_system
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_file_system
#include"interface/can_callback.hpp"
#include"lang/cxx.hpp"
#include"memop/copy.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::draft_file_system{
    enum default_map_t{
        page_of_file_system_header      = 0,
        page_of_entry                   = 1,
        page_of_allocator               = 2,
        page_of_root_dir                = 3,
        page_of_linear_begin            = 8,
    };

    struct page_group{
        u32                 page_lv[7]              = {0};
        u32                 page_table              =  0;
    };

    struct file_header{
        u32                 bytes;
        u32                 create_time;
        u32                 modify_time;
        u32                 reserved        : 27;
        u32                 write_times     : 5;
        page_group          content;

        // 512 512 1K  2K
        // 4K  8K  16K 32K
    };

    struct name_item{
        u16                 owner_index;
        u08                 name[2]; // 连续的 2 * n 个字节
    };

    struct direcotry_header{
        u32                 create_time;
        u32                 reserved        : 25    = 0;
        u32                 write_times     : 6     = 0;
        u32                 reservedx[6]            = {0};

        struct subset{
            u32             count                   = 0;
            u32             redirection_count       = 0;
            u32             name_list               = 0;
            u32             reserved                = 0;
            page_group      header;
        } file, subdir;
    };

    struct free_page{
        u32                 next                    = 0;
        u32                 items[512 / sizeof(u32) - 1];
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

    constexpr inc::c08 capacity_too_small           = "capacity too small";
    constexpr inc::c08 name_too_long                = "name too long";
    constexpr inc::c08 success                      = "";

    struct file_system{
        using access_invoke = inc::can_callback<void(u32 page, voidp buffer, u64 bytes)>;

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

            write(page_of_root_dir, xref root_dir, sizeof(root_dir));
            write(page_of_allocator, xref allocator, sizeof(allocator));
            write(page_of_entry, xref entry, sizeof(entry));
            write(page_of_file_system_header, xref header, sizeof(header));
            return success;
        }

        void create_dir(direcotry_header & parent, inc::c08 name){

        }

        auto create_file(inc::c08 const * path, uxx length){

        }

        void write(u32 page, voidp buffer, u64 bytes){
            writer(page, buffer, bytes);
        }

        void read(u32 page, voidp buffer, u64 bytes){
            read(page, buffer, bytes);
        }

        static u32 bytes_to_page(u64 bytes){

        }

        static u32 now(){

        }
    };

}

#endif
