#define xuser   mixc::powerful_cat
#include"algo/hash.hpp"
#include"define/base_type.hpp"
#include"lang/cxx/find.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx.hpp"
#include"memop/cast.hpp"
#include"memop/copy.hpp"
#include"memop/swap.hpp"
#define inc     xuser

#define xread(block,field,...)                              \
    this->meta_partion_read(                                \
        block,                                              \
        __VA_ARGS__ + offsetof(& meta_dir_header::field),   \
        xref(field),                                        \
        sizeof(field)                                       \
    )

#define xwrite(block,field,...)                             \
    this->meta_partion_write(                               \
        block,                                              \
        __VA_ARGS__ + offsetof(& meta_dir_header::field),   \
        xref(field),                                        \
        sizeof(field)                                       \
    )

/*
nor flash + nand flash 结合
nor flash 进行 meta 信息记录
nand flash 进行批量数据存储
*/

constexpr u32 bytes_of_block    = 64;           // 元数据块基本分配为 64 字节
constexpr u32 avaliable_token   = 0x76543210;   // 当闪存制作好文件系统后设置该值
constexpr uxx max_name_length   = 256;
constexpr uxx max_path_deepth   = 256;

typedef struct address_in_disk{
    u32     any         : 6;
    u32     block       : 26;

    bool is_exist(){
        return block != 0;
    }
} aid;

typedef struct meta_create_time{
    u08     create_time[6];
} mct;

struct meta_base_header{
    u32     owner_id;
    aid     next;
    u32     self_id;
    aid     left;

    // 当前节点不是子目录/文件列表尾元时，该联合体表示 right，下一个节点
    // 否则表示 total，当前链表的长度
    union{
        aid right;
        u32 total;
    };

    aid     prev;

    mct     create_time;
    u08     write_times;
private:
    u08     reserved;

public:
    u08     name_length;
    u08     name[15];
    aid     name_next;
};

struct meta_name_block{
    u08     name[bytes_of_block - sizeof(aid)];
    aid     name_next;
};

struct meta_file_header : meta_base_header{
    aid     content;
    u08     bytes[6];
private:
    u16     reserved;
};

struct meta_dir_header : meta_base_header{
    aid     file_list;
    aid     dir_list;
private:
    u32     reserved;
};

struct meta_multi_table{
    aid     items[15];

    // 约定：
    // 该字段也是 item[] 的一部分，在 item 未存满时用于存放链表元素总数
    // meta_multi_table::items 指向下一级 meta_multi_table
    // 只有最上级 meta_multi_table 存在 total_count
    u32     total_count;
};

typedef struct block_range{
    aid     begin;
    aid     end;
} br;

struct meta_header{
    u08     fs_name[16] = "Mix-C HFS";
    u32     version;
    u32     id;
    u08     is_meta_partion     : 1;

    // 
    u08     mass_partion_page_scale;

    u32     reserved[16];

    br      reserved_area;
    br      hash_area;
    br      linear_area;

    aid     meta_space_info;
    u32     avaliable_token;
};

struct meta_recycle_list{
    aid     prev;
    aid     next;
};

struct meta_space_info{
    aid     linear_cursor;
    aid     recycle_list;
};

struct mem_handler{
    aid     image;
    u32     id;

    bool is_exist(){
        // 约定：
        // 通过 mem_handler{} 初始化，默认该块不存在
        return image.block != 0;
    }
};

struct filehub{
    bstate_t write(aid block, const void * data, uxx bytes){

    }

    bstate_t read(aid block, void * buffer, uxx bytes){

    }

    aid meta_hash_area_read(uxx index){

    }

    void meta_hash_area_write(uxx index, aid value){

    }

    void meta_partion_read(aid block, uxx offset, void * buffer, uxx bytes){
        
    }

    void meta_partion_write(aid block, uxx offset, void const * buffer, uxx bytes){
        
    }

    template<class stream_t>
    bstate_t read(aid block, void * buffer, uxx bytes, stream_t const & stream){
        
    }

    template<class stream_t>
    bstate_t read(aid block, uxx bytes_offset, void * buffer, uxx bytes, stream_t const & stream){
        
    }

    mem_handler open(mem_handler parent, inc::c08 name, bool open_create){
        // TODO：
        if (name.length() >= max_name_length){

        }

        // 场景：
        // 1.当前 hash 首节点无元素
        // 2.当前 hash 首节点有元素，插入到链表表首
        // 3.当前 hash 首节点有元素，插入到链表表中
        // 4.当前 hash 首节点有元素，插入到链表表尾
        auto owner_id           = u32{};
        auto self_id            = u32{};
        auto name_length        = u08{};
        auto next               = aid{};
        auto prev               = aid{};
        auto i                  = this->hash(name, parent.id);
        auto head               = this->meta_hash_area_read(i);

        for(next = head; next.is_exist(); prev = next, xread(next, next)){
            // 初次判断，如果当前节点的所有者不是 parent，就找下一个元素
            // 约定：
            // owner_id 在哈希链表中是递增的，下一个 owner_id 大于前一个 owner_id
            // 如果当前 owner_id > parent.id 则表示没找到匹配项
            // 因为后面的 owner_id 都会大于 parent.id
            if (xread(next, owner_id); owner_id < parent.id){
                continue;
            }
            else if (owner_id < parent.id){
                break;
            }

            // 如果名称长度不匹配就不用再与比较名称了
            if (xread(next, name_length); name_length != name.length()){
                continue;
            }

            // 由于 name 在文件系统上是以链表形式分块存储的
            // 且首块 name 的大小不足一个单位块的大小
            auto next_name      = next;
            auto current        = next;
            auto end            = sizeof(meta_base_header::name);
            auto name_offset    = offsetof(& meta_base_header::name);
            auto next_offset    = offsetof(& meta_base_header::name_next);
            auto item           = u08{};
            auto i              = uxx{};
            auto j              = uxx{};

            // 判断名称是否与参数 name 符合
            for(; i < name_length; i++, j++){
                // 到达当前 name 块的末尾，通过 next_name 获取下一块 name
                if (j == end){
                    this->meta_partion_read(next_name, next_offset, xref(next_name), sizeof(next_name));
                    end         = sizeof(meta_name_block::name);
                    name_offset = offsetof(& meta_name_block::name);
                    next_offset = offsetof(& meta_name_block::name_next);
                    j           = 0;
                }

                if (this->meta_partion_read(next_name, name_offset + j, xref(item), sizeof(item));
                    item != name[i]){
                    break;
                }
            }

            // 如果名称符合，就返回当前块的基本信息
            if (i == name_length){
                xread(next, self_id); return mem_handler{
                    .image = current,
                    .id = self_id,
                };
            }
        }

        if (open_create == false){
            return mem_handler{};
        }

        // 如果不存在该目录/文件时创建
        // 创建时需要将当前目录/文件添加到父目录下
        auto is_dir             = name[-1] == '/';
        auto offset             = is_dir ? uxx{} : 
            offsetof(& meta_dir_header::file_list) - offsetof(& meta_dir_header::dir_list);
        auto new_item           = this->alloc_block();
        auto left               = aid{};
        auto right              = new_item;
        auto dir_list           = aid{};
        auto total              = u32{};
        auto meta               = meta_dir_header{};
        meta.owner_id           = parent.id;
        meta.next               = next;
        meta.self_id            = this->new_id();

        // 当 is_dir = true 时，此时 offset 为 0，dir_list 表示子目录链表
        // 当 is_dir = false 时，此时 offset 不为 0，dir_list + offset 指向 file_list，表示当前目录文件列表
        if (xread(parent.image, dir_list, offset); dir_list.is_exist()){
            xread(dir_list, left);
            xread(dir_list, total);     // 先读取 total
            xwrite(dir_list, right);    // 再写入 right，避免写入 right 后覆盖 total 值
        }

        meta.left               = dir_list;
        meta.total              = total + 1;
        meta.prev               = prev;
        meta.create_time        = this->now();
        meta.write_times        = 0;
        meta.name_length        = name.length();
        meta.name_next          = aid{};

        if (name.length() <= sizeof(meta_base_header::name)){
            inc::copy_unsafe(meta.name, name, name.length());
        }
        // 需要创建文件名块链表
        else{
            aid  buffer[max_name_length / sizeof(meta_name_block::name) + 2]{};
            auto i              = uxx{};
            auto step           = uxx{};
            auto mnb            = meta_name_block{};
            inc::copy_unsafe(meta.name, name, sizeof(meta.name));

            for(ixx rest = ixx(name.length() - sizeof(meta.name)); rest > 0; rest -= sizeof(mnb.name)){
                buffer[i++]     = this->alloc_block();
            }

            for(meta.name_next = buffer[0], 
                i = 0,
                name = name.backward(sizeof(meta.name));
                name.length() != 0;
                name = name.backward(step),
                i++
            ){
                step            = name.length() <= sizeof(mnb.name) ? name.length() : sizeof(mnb.name);
                mnb.name_next   = buffer[i + 1];
                inc::copy_unsafe(mnb.name, name, step);
                this->meta_partion_write(buffer[i], 0, & mnb, sizeof(mnb));
            }
        }

        // 将 meta 刷新到磁盘
        // 如果需要插入到链表表首
        if (this->meta_partion_write(new_item, 0, & meta, sizeof(meta)); head.block == prev.block){
            this->meta_hash_area_write(i, new_item);
        }
        // 追加到链表上
        else{
            next                = new_item;
            xwrite(prev, next);
        }

        // 新节点作为链表尾元
        dir_list                = new_item;
        xwrite(parent.image, dir_list, offset);

        return mem_handler{
            .image = new_item,
            .id = meta.self_id,
        };
    }

    mem_handler open_file(inc::c08 path, bool open_create = false){
        // 至少包含一个 '/' 和一个字符的名称
        if (path.length() <= 2){
            return mem_handler{};
        }

        auto i_end              = path.index_of_last('/');
        auto parent             = mem_handler{};

        // 缺失文件名
        // path 不存在 '/' 或者存在于最后一个字符
        if (i_end >= path.length() - 1){
            return mem_handler{};
        }

        // 可能没有目录直接到文件   /file
        // 可能经过目录再到文件     /dir/.../file
        auto folder_path        = inc::c08{path}.length(i_end + 1);
        auto file_name          = inc::c08{path}.backward(i_end + 1);

        if (file_name.length() >= max_name_length){
            return mem_handler{};
        }

        if (auto parent = this->open_dir(folder_path, open_create); parent.is_exist()){
            return this->open(parent, file_name, open_create);
        }
        else{
            return mem_handler{};
        }
    }

    mem_handler open_dir(inc::c08 path, bool open_create = false){
        /*
        格式要求：
        - 只接受全路径，不接受 .. 或 . 
        - 最后一个字符必须为 /

        path 格式：
        /folder_0/.../folder_n/
         */

        if (path.length() == 0){
            return mem_handler{};
        }
        if (path[0] != '/' or path[-1] != '/'){
            return mem_handler{};
        }
        if (path.length() == 1){
            return this->root_dir();
        }
        else{
            path                = path.backward(1);
        }

        auto i_begin            = uxx{};
        auto i_end              = uxx{};
        auto folder             = inc::c08{};
        auto parent             = this->root_dir();

        path.find('/', [&](uxx index){
            if (i_end           = index, 
                folder          = inc::c08{path}.backward(i_begin).length(i_end - i_begin + 1/*保留末尾 '/' */);
                i_end != i_begin
            ){
                parent          = this->open(parent, folder, open_create);
            }

            i_begin             = i_end + 1;    // 跳到 '/' 后边一个位置
            i_end               = i_begin;
            return parent.is_exist() ? loop_t::go_on : loop_t::finish;
        });

        return parent;
    }

    mem_handler root_dir(){

    }

    uxx bytes_per_buffer(){

    }

    aid alloc_block(){
        // 约定：
        // 返回的 aid::any 部分为 0
    }

    u32 hash(inc::c08 name, u32 parent = 0){
        auto value              = u32(inc::hash(name, parent));
        auto masked             = value & m_hash_mask;
        auto i                  = 
            masked >= m_hash_area_item_count ? 
            masked - m_hash_area_item_count : masked;
        return i;
    }
private:
    template<class member_ptr_t>
    static uxx offsetof(member_ptr_t const & ptr){
        return *uxxp(& ptr);
    }

    static meta_create_time now(){
        // 2^48 
        meta_create_time time{};
        return time;
    }

    uxx new_id(){
        return m_id++;
    }

    u32     m_id;
    u32     m_hash_mask;
    u32     m_hash_area_item_count;
    aid     m_hash_area;
};

