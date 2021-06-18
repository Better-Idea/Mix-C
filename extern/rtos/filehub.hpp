#define xuser   mixc::powerful_cat
#include"define/base_type.hpp"
#include"lang/cxx/find.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx.hpp"
#include"math/index_system.hpp"
#define inc     xuser

typedef struct address_in_disk{
    u32         page;
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
    } address;

    struct{
        u16     major;
        u16     minor;
    } version;

    u08         scale_block;
    u08         bmp_ability[8];
};

struct dir_header{
    u64         timestamp_created : 48;
    u08         name_length;
    u08         name[23];

    struct{
        u32     length;
        aid     address;
    } file, subdir;
};

struct fs_item_digest{
    u32         hash;
    u32         address;
};

struct file_header{
    u64         timestamp_created : 48;
    u08         name_length;
    u08         name[23];
    u64         reserved0;
    u64         bytes : 48;
    u32         reserved1[4];

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
    aid         address;

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
    bstate_t write(aid address, const void * data, uxx bytes){

    }

    bstate_t read(aid address, void * buffer, uxx bytes){

    }

    meta_file open_file(inc::c08 path, bool create_when_not_existence = false){
        // 至少包含一个 '/' 和一个字符的名称
        if (path.length() <= 2){
            return meta_file{};
        }

        auto i_end          = path.index_of_last('/');
        auto parent         = meta_dir{};

        // 缺失文件名
        // path 不存在 '/' 或者存在于最后一个字符
        if (i_end >= path.length() - 1){
            return meta_file{};
        }

        // 可能没有目录直接到文件   /file
        // 可能经过目录再到文件     /dir/.../file
        auto folder_path    = inc::c08{path}.length(i_end);
        auto file_name      = inc::c08{path}.backward(i_end + 1);
        auto parent         = this->open_dir(folder_path, create_when_not_existence);

        if (not parent.is_exist()){
            return meta_file{};
        }
        return this->open_file(parent, file_name, create_when_not_existence);
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
            return;
        }
        else{
            path            = path.backward(1);
        }

        auto i_begin        = uxx(0);
        auto i_end          = uxx(0);
        auto folder         = inc::c08{};
        auto parent         = this->root_dir();

        path.find('/', [&](uxx index){
            if (i_end       = index, 
                folder      = inc::c08{path}.backward(i_begin).length(i_end - i_begin);
                folder.is_empty() == false
            ){
                parent      = this->open_dir(parent, folder, create_when_not_existence);
            }

            i_begin         = i_end + 1;    // 跳到 '/' 后边一个位置
            i_end           = i_begin;
            return parent.is_exist() ? loop_t::go_on : loop_t::finish;
        });

        if (parent.is_exist()){
            parent          = this->open_dir(parent, folder, create_when_not_existence);
        }
        return parent;
    }

    meta_dir root_dir(){

    }

    meta_dir open_dir(meta_dir parent, inc::c08 folder_name, bool create_when_not_existence = false){

    }

    meta_file open_file(meta_dir parent, inc::c08 file_name, bool create_when_not_existence = false){

    }
};

void main(){
    
}
