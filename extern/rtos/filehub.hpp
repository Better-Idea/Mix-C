#define xuser   mixc::powerful_cat
#include"algo/hash.hpp"
#include"define/base_type.hpp"
#include"extern/rtos/task.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx/find.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx.hpp"
#include"math/index_system.hpp"
#define inc     xuser

constexpr uxx is_file   = 0;
constexpr uxx is_dir    = 1;

typedef struct address_in_disk{
    u32         page;

    bool is_exist(){
        return page != 0;
    }
} aid;

struct fs_header{
    u08         name[8] = { 'M', 'i', 'x', '-', 'C', ' ', 'F', 'S' };
    u64         magic_number;

    struct{
        u64     total;
        u64     used;
    } byte;

    struct{
        aid     root;
        aid     hub;
    } block;

    struct{
        u16     major;
        u16     minor;
    } version;

    u08         scale_block;
    u08         bmp_ability[8];
};

struct dirfile_header{
    u64         timestamp_created;
    u08         name_length;
    u08         name[23];
    u32         reserve[4];
};

typedef struct dir_header : dirfile_header{
    u32         number_of_file;
    u32         number_of_subdir;
    u16         number_of_deleted_file;
    u16         number_of_deleted_subdir;
    u32         reserve;

    // content as follow ...
    // fs_item_digest
} * dir_headerp;

typedef struct fs_item_digest{
    u32         is_file     : 1;
    u32         hash        : 31;
    aid         block;
} * fs_item_digestp;

typedef struct fs_item_linker{
    aid         next;
    u32         dummy;
} * fs_item_linkerp;

struct file_header : dirfile_header{
    u32         reserved[2];
    u64         bytes;

    // content as follow ...
};

enum{
    a = sizeof(file_header),
    b = sizeof(dir_header),
    s = sizeof(fs_header),
};

/*
nor flash + nand flash 结合
nor flash 进行 meta 信息记录
nand flash 进行批量数据存储
*/

struct meta_file{
    bool is_exist(){
        return true;
    }
};

// 4KB
struct meta_dir{
    aid         block;

    bool is_exist(){
        return true;
    }
};

struct meta_record{
    aid         free_linear;
    aid         free_table;
    aid         free_table_length;
};

struct filehub{
    bstate_t write(aid block, const void * data, uxx bytes){

    }

    bstate_t read(aid block, void * buffer, uxx bytes){

    }

    template<class stream_t>
    bstate_t read(aid block, void * buffer, uxx bytes, stream_t const & stream){
        
    }

    template<class stream_t>
    bstate_t read(aid block, uxx bytes_offset, void * buffer, uxx bytes, stream_t const & stream){
        
    }

    meta_file open_file(inc::c08 path, bool create_when_not_existence = false){
        // 至少包含一个 '/' 和一个字符的名称
        if (path.length() <= 2){
            return meta_file{};
        }

        auto i_end              = path.index_of_last('/');
        auto parent             = meta_dir{};

        // 缺失文件名
        // path 不存在 '/' 或者存在于最后一个字符
        if (i_end >= path.length() - 1){
            return meta_file{};
        }

        // 可能没有目录直接到文件   /file
        // 可能经过目录再到文件     /dir/.../file
        auto folder_path        = inc::c08{path}.length(i_end);
        auto file_name          = inc::c08{path}.backward(i_end + 1);
        auto parent             = this->open_dir(folder_path, create_when_not_existence);

        if (not parent.is_exist()){
            return meta_file{};
        }

        this->open(parent, file_name, is_file, create_when_not_existence);
    }

    meta_dir open_dir(inc::c08 path, bool create_when_not_existence = false){
        /*
        只接收全路径
        path 格式：
        /folder_0/.../folder_n/file
         */
        
        if (path.length() == 1){
            return this->root_dir();
        }
        if (path.length() < 1){
            return meta_dir{};
        }
        if (path[0] != '/'){
            return meta_dir{};
        }
        else{
            path                = path.backward(1);
        }

        auto i_begin            = uxx(0);
        auto i_end              = uxx(0);
        auto folder             = inc::c08{};
        auto parent             = this->root_dir();

        path.find('/', [&](uxx index){
            if (i_end           = index, 
                folder          = inc::c08{path}.backward(i_begin).length(i_end - i_begin);
                folder.is_empty() == false
            ){
                parent          = this->open(parent, folder, is_dir, create_when_not_existence);
            }

            i_begin             = i_end + 1;    // 跳到 '/' 后边一个位置
            i_end               = i_begin;
            return parent.is_exist() ? loop_t::go_on : loop_t::finish;
        });

        if (parent.is_exist()){
            parent          = this->open(parent, folder, is_dir, create_when_not_existence);
        }
        return parent;
    }

    meta_dir root_dir(){

    }

    meta_dir open(meta_dir parent, inc::c08 folder_name, uxx type, bool create_when_not_existence = false){
        // 分配缓冲区，实际大小比 sizeof(dir_header) 大
        auto child              = meta_dir{};
        auto buf_parent         = this->buffer_parent();
        auto buf_child          = this->buffer_child();
        auto hash               = this->hash(folder_name);
        auto block              = parent.block;
        auto next               = aid{};
        auto handle             = [&](uxx i_buf_block){
            auto item           = fs_item_digestp(buf_parent);
            auto end            = fs_item_digestp(buf_parent + this->bytes_per_buffer());

            if (i_buf_block != 0){
                ; // pass
            }
            else if (block.page == parent.block.page){
                item           += sizeof(dir_header) / sizeof(fs_item_digest) + 1/**/;
                next            = fs_item_linkerp(item - 1)->next;
            }
            else{
                item           += 1;
                next            = fs_item_linkerp(item - 1)->next;
            }

            for(;; item += 1){
                if (item != end or item->block.is_exist() == false){
                    return loop_t::go_on;
                }

                if (item->is_file != type or hash != item->hash){
                    continue;
                }

                // 目前名称的 hash 值匹配，还需要判断当前子目录真实名称是否匹配
                static_assert(sizeof(dir_header) == sizeof(file_header));
                this->read(item->block, buf_child, sizeof(dir_header)/*sizeof(file_header)*/);

                // TODO：匹配名称=======================================================
                if (true){
                    child.block = item->block;
                    return loop_t::finish;
                }
            }

        };

        // 流式读取
        while(true){
            if (this->read(block, buf_parent, this->bytes_per_buffer(), handle); 
                child.is_exist()){
                return child;
            }

            if (next.is_exist()){
                continue;
            }
            if (create_when_not_existence == false){
                return meta_dir{};
            }
        }
    }

    uxx bytes_per_block(){
        return 512;
    }

    uxx bytes_per_buffer(){

    }

    u08p buffer_parent(){

    }

    u08p buffer_child(){

    }

    aid alloc_block(){

    }

    u32 hash(inc::c08 name){
        return u32(hash(name)) >> 1;
    }
};

void main(){
    
}
