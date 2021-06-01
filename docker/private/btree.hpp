// TODO: 特化 foreach
#ifndef xpack_docker_btree
#define xpack_docker_btree
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_btree::inc
#include"concurrency/lock/atom_swap.hpp"
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"docker/private/adapter.array_access.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/mirror.hpp"
#include"dumb/move.hpp"
#include"instruction/index_of_first_set.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"memop/swap.hpp"
#include"utils/allocator.hpp"
#include"utils/bits_indexer.hpp"

#if xis_msvc_native
#include<intrin.h>
#define __m256i_u  __m256i
#else
#include<x86intrin.h>
#endif
#pragma pop_macro("xuser")

namespace mixc::docker_btree{
    constexpr bool is_full  = true;
    constexpr bool is_empty = true;

    template<class item_t>
    struct item_group{
        enum{ 
            length          = 16, 
            group_max       = 4
        };

        using m_t           = inc::mirror<item_t>;

        struct item_node{
            // 逻辑地址，由于该字段为 64bit，可以构成 16 个 4bit 的位组
            // 我们可以用这些位组来存下标编号，在不使用该字段前
            // 我们要插入一个元素需要将该插入点后边的所有元素都往后挪
            // 现在我们只要挪 indexer 中表示索引的位组就好了
            // 假设 indexer 初始值 为 0x0123456789abcdef
            // 那么往下标 1 插入到元素时，我们把最后一个元素占用的空间挪出去
            // 用于存放插入元素，那么此时 index 为 0x123456789abcde0f
            inc::bits_indexer<4>        indexer;
            item_t                      item[length];
        };

        item_node * groups[group_max]       = {};
        uxx         count                   = 0;

        bool insert(uxx i, item_t const & value){
            // 带构造的初始化一次，但 m 析构时不析构 item_t
            m_t         m                   = { value, inc::construction_t::execute };
            uxx         i_group             = i >> 4;       // 插入的索引 i 在 groups 的第几组
            uxx         i_offset            = i & 0xf;      // 插入的索引 i 在对应组中的偏移
            uxx         i_group_max         = count >> 4;   // 当前元素个数最大组号
            uxx         i_free;                             // 插入一个元素到一个组时，如果改组存满（已经存了 16 个元素）
                                                            // 那么需要该组最后一个元素挪到下一组第一个元素
            item_node * group;

            for(uxx i = i_group; i <= i_group_max; i++){
                if (groups[i] == nullptr){
                    groups[i]               = inc::alloc_with_initial<item_node>(
                        inc::memory_size{sizeof(item_node)}
                    );
                    group                   = groups[i];
                    i                       = group_max;    // 当前组为空时，那么就无需再往后挪元素了
                }
                else{
                    group                   = groups[i];
                }

                // 该组最后一个元素挪出去后，就空出一个空间，该空间可以用来存插入的元素
                i_free                      = group->indexer.get(15/*最后一个*/);
                inc::swap(xref m, (m_t *)(xref group->item[i_free]));
                group->indexer.insert(i_offset, i_free);

                // 之后每次都是把上次挪出去的最后一个元素插入到下一组的第一个位置
                i_offset                    = 0;
            }

            // 存满返回 true
            count                          += 1;
            return count == group_max * length;
        }

        bool talk_out(uxx i, item_t * value){
            // 先减计数
            count                          -= 1;

            // 当 count 值为 0, 16, 32, 48 时需要释放最后一个组，因为通过该函数带出一个元素后
            // 最后一个组已经空了
            bool        free_last           = (count & 0xf) == 0;
            m_t         m;
            uxx         i_group             = i >> 4;
            uxx         i_group_max         = count >> 4;
            uxx         i_offset            = i & 0xf;
            uxx         i_move;
            uxx         i_free;
            item_node * group;

            // 从后往前挪动
            for(uxx i = i_group_max; ixx(i) >= ixx(i_group); i--){
                // 当 i 到达取出元素对应的组时，i_move 变成取出点对应的索引
                // 否则每次取出当前组第一个元素放到前一组的最后一个位置
                i_move                      = i != i_group ? 0 : i_offset;
                group                       = groups[i];
                i_free                      = group->indexer.get(i_move);
                group->indexer.remove(i_move);
                group->indexer.set(15, i_free);
                inc::swap(xref m, (m_t *)(xref group->item[i_free]));
            }

            if (free_last){
                inc::free(groups[i_group_max], inc::memory_size{
                    sizeof(item_node)
                });
                groups[i_group_max]         = nullptr;
            }

            if (value != nullptr){
                value[0]                    = inc::move((item_t &)m); // 移动语义
            }

            // 析构
            m->~item_t();
            return count == 0;
        }

        item_group * split(){
            // brother 节点用于插入到当前节点的前面
            item_group<item_t> * brother    = inc::alloc_with_initial<item_group>(
                inc::memory_size{sizeof(item_group<item_t>)}
            );

            // group_max * length 为存满时的元素个数
            // 执行 split 操作时进行分半
            brother->count                  = group_max * length / 2;
            brother->groups[0]              = this->groups[0];
            brother->groups[1]              = this->groups[1];
            this->groups[0]                 = this->groups[2];
            this->groups[1]                 = this->groups[3];
            this->groups[2]                 = nullptr;
            this->groups[3]                 = nullptr;
            this->count                     = group_max * length / 2;
            return brother;
        }

        item_t & get(uxx i){
            uxx         i_group             = i >> 4;
            uxx         i_offset            = i & 0xf;
            item_node * group               = groups[i_group];
            uxx         i_real_offset       = group->indexer.get(i_offset);
            return group->item[i_real_offset];
        }

        void clear(){
            auto page = count / 16 + (count % 16 != 0);

            for(uxx i = 0; i < count; i++){
                get(i).~item_t();
            }

            for(uxx i = 0; i < page; i++){
                inc::free(groups[i], inc::memory_size{
                    sizeof(item_node)
                });
                groups[i]                   = nullptr;
            }
            count                           = 0;
        }
    };

    template<class item_t>
    struct path_group{
        using item_node                         = item_group<item_t>;

        // 注意：btree::clear 中要求该字段排在首位以便和 path_group<T> 可以进行强制转换
        union{
            item_node             * items [8]   = {}; 
            path_group<item_t>    * bottom[8];
        };
        i32                         offset[8]   = {};
    };

    inline const i32 null_node = 0;

    template<class final_t, class item_type>
    xstruct(
        xtmpl(btree, final_t, item_type),
        xpubb(inc::disable_copy),
        xasso(item_type)
    )
    public:
        using item_t                        = item_type;
    private:
        using path_node                     = path_group<item_t>;
        using item_node                     = typename path_node::item_node;

        path_node   *   root                = null();

        static path_node * null(){
            // 用 null() 代替 nullptr，让 btree::length() 可以在只有 0 个元素的时候可访问
            uxx     offset                  = uxx(& ((path_node *)nullptr)->offset[7]);
            u08p    ptr                     = u08p(& null_node) - offset;
            return (path_node *)ptr;
        }

        static item_node * mark_is_item_level(voidp origin){
            uxx marked                      = uxx(origin) | 1;
            return (item_node *)marked;
        }

        static item_node * unmark(voidp marked){
            uxx origin                      = uxx(marked) & uxx(-2);
            return (item_node *)origin;
        }

        static bool is_item_level(voidp may_marked){
            return (uxx(may_marked) & 1) != 0;
        }

        static item_node * alloc_item_node(){
            // TODO: inc::alloc_with_initial 暂时还不支持超过 16 字节的对齐内存分配
            // 目前该接口使用 AVX 在 5M 数据随机插入性能 大约有 7% 的性能差距
            // 测试       ：5M 数据随机插入 | 读取
            // 32Byte 对齐：提升约 7%       | 提升约 7%
            return inc::alloc_with_initial<item_node>(
                inc::memory_size{sizeof(item_node)}
            );
            // return new item_node;
        }

        static path_node * alloc_path_node(){
            return inc::alloc_with_initial<path_node>(
                inc::memory_size{sizeof(path_node)}
            );
            // return new path_node;
        }
        
    protected:
        ~btree(){
            clear();
        }
    public:
        void clear(){
            auto cur                        = null();

            if (cur = inc::atom_swap(xref root, cur); cur == null()){
                return;
            }

            path_node **   path[32];
            uxx            i_path[32];
            path_node ***  path_ptr         = path;
            uxx       *    i_path_ptr       = i_path;
            i_path_ptr[0]                   = 0;
            path_ptr[0]                     = cur->bottom;

            while(path_ptr >= xref path[0]){
                auto i                      = (i_path_ptr[0]);
                auto arrive_end             = (i == 8);
                auto arrive_item_level      = (false);

                if (not arrive_end){
                    cur                     = (path_ptr[0][i]);
                    arrive_item_level       = (is_item_level(cur));
                    arrive_end              = (cur == nullptr);
                }
                if (arrive_end){
                    inc::free((path_node *)path_ptr[0]); // 要释放的大小为 sizeof(path_node)
                    i_path_ptr             -= 1;
                    path_ptr               -= 1;
                    continue;
                }
                else{
                    i_path_ptr[0]          += 1;
                }

                if (not arrive_item_level){
                    i_path_ptr             += 1;
                    i_path_ptr[0]           = 0;
                    path_ptr               += 1;
                    path_ptr[0]             = cur->bottom;
                }
                else{
                    auto vals               = unmark(cur);
                    vals->clear();
                    inc::free(vals);
                }
            }
        }

        void remove(uxx index){
            talk_out(index, nullptr);
        }

        void insert(uxx index, item_t const & value){
            if (item_node * item_ptr; root == null()){
                root                        = alloc_path_node();
                item_ptr                    = alloc_item_node();
                root->items[0]              = mark_is_item_level(item_ptr);
            }

            uxx         iofsg[32];
            path_node * path[32];
            auto path_ptr                   = (path);
            auto iofsg_ptr                  = (iofsg);
            auto i                          = (i32)index;
            auto cur                        = (root);
            auto iofs                       = (uxx)0;
            auto new_node                   = (path_node *)nullptr;
            auto new_item                   = (item_node *)nullptr;
            auto parent                     = (path_node *)nullptr;
            auto offset                     = (0);

            while(true){
                if (not is_item_level(cur)){
                    auto pi                 = _mm256_set1_epi32(i);
                    auto pcum               = _mm256_loadu_si256((__m256i_u *)cur->offset);
                    auto pcmpgt             = _mm256_cmpgt_epi32(pcum, pi);
                    auto pcmpeq             = _mm256_cmpeq_epi32(pcum, pi);
                    auto pmsk               = _mm256_movemask_ps(_mm256_castsi256_ps(pcmpgt));
                    auto padd               = _mm256_sub_epi32(pcum, pcmpgt); // 减去 -1 -> +1

                    if (pmsk){
                        iofs                = inc::index_of_first_set(pmsk);
                    }
                    else{
                        iofs                = _mm256_movemask_ps(_mm256_castsi256_ps(pcmpeq));
                        iofs                = inc::index_of_first_set(iofs);
                        padd                = _mm256_sub_epi32(pcum, pcmpeq);
                    }
                    if (iofs > 0){
                        i                  -= cur->offset[iofs - 1];
                    }

                    _mm256_storeu_si256((__m256i *)cur->offset, padd);
                    iofsg_ptr[0]            = iofs;
                    path_ptr[0]             = cur;
                    parent                  = cur;
                    iofsg_ptr              += 1;
                    path_ptr               += 1;
                    cur                     = cur->bottom[iofs];
                }
                else{
                    break;
                }
            }

            if (auto vals = unmark(cur); vals->insert(i, value) != is_full){
               return;
            }
            else{
               new_item                    = vals->split();
               offset                      = new_item->count;
               new_item                    = mark_is_item_level(new_item);
               new_node                    = (path_node *)new_item;
            }

            while(path_ptr != xref path[0]){
               iofsg_ptr                  -= 1;
               path_ptr                   -= 1;
               iofs                        = iofsg_ptr[0];
               parent                      = path_ptr[0];
               offset                     += iofs ? parent->offset[iofs - 1] : 0;
               auto need_return            = parent->bottom[7] == nullptr;
               auto node_temp              = new_node;

               while(iofs < 8 and node_temp != nullptr){
                   inc::swap(xref parent->offset[iofs], xref offset); 
                   inc::swap(xref parent->bottom[iofs], xref node_temp);
                   iofs                   += 1;
               }

               if (need_return){
                   return;
               }

               new_node                    = alloc_path_node();
               new_node->bottom[0]         = parent->bottom[0];
               new_node->bottom[1]         = parent->bottom[1];
               new_node->bottom[2]         = parent->bottom[2];
               new_node->bottom[3]         = parent->bottom[3];
               new_node->bottom[4]         = nullptr;
               new_node->bottom[5]         = nullptr;
               new_node->bottom[6]         = nullptr;
               new_node->bottom[7]         = nullptr;
               parent->bottom[0]           = parent->bottom[4];
               parent->bottom[1]           = parent->bottom[5];
               parent->bottom[2]           = parent->bottom[6];
               parent->bottom[3]           = parent->bottom[7];
               parent->bottom[4]           = node_temp;
               parent->bottom[5]           = nullptr;
               parent->bottom[6]           = nullptr;
               parent->bottom[7]           = nullptr;

               // 转移偏移
               new_node->offset[0]         = parent->offset[0];
               new_node->offset[1]         = parent->offset[1];
               new_node->offset[2]         = parent->offset[2];
               new_node->offset[3]         = parent->offset[3];
               new_node->offset[4]         = parent->offset[3];
               new_node->offset[5]         = parent->offset[3];
               new_node->offset[6]         = parent->offset[3];
               new_node->offset[7]         = parent->offset[3];

               parent->offset[0]           = parent->offset[4] - parent->offset[3];
               parent->offset[1]           = parent->offset[5] - parent->offset[3];
               parent->offset[2]           = parent->offset[6] - parent->offset[3];
               parent->offset[3]           = parent->offset[7] - parent->offset[3];
               parent->offset[4]           = parent->offset[3] + offset - parent->offset[7];
               parent->offset[5]           = parent->offset[4];
               parent->offset[6]           = parent->offset[4];
               parent->offset[7]           = parent->offset[4];
               offset                      = new_node->offset[7];
            }

            auto new_root                   = alloc_path_node();
            new_root->bottom[0]             = new_node;
            new_root->bottom[1]             = root;
            new_root->bottom[2]             = nullptr;
            new_root->bottom[3]             = nullptr;
            new_root->bottom[4]             = nullptr;
            new_root->bottom[5]             = nullptr;
            new_root->bottom[6]             = nullptr;
            new_root->bottom[7]             = nullptr;

            new_root->offset[0]             = offset;
            new_root->offset[1]             = offset + root->offset[7];
            new_root->offset[2]             = new_root->offset[1];
            new_root->offset[3]             = new_root->offset[1];
            new_root->offset[4]             = new_root->offset[1];
            new_root->offset[5]             = new_root->offset[1];
            new_root->offset[6]             = new_root->offset[1];
            new_root->offset[7]             = new_root->offset[1];
            root                            = new_root;
        }

        void talk_out(uxx index, item_t * value){
            path_node * path[32];
            uxx  iofsg[32];
            auto path_ptr                   = (path);
            auto iofsg_ptr                  = (iofsg);
            auto cur                        = (root);
            auto parent                     = (root);
            auto i                          = (i32)index;
            auto iofs                       = (0);

            while(true){
                if (not is_item_level(cur)){
                    auto pi                 = _mm256_set1_epi32(i);
                    auto pcum               = _mm256_loadu_si256((__m256i_u *)cur->offset);
                    auto pcmpgt             = _mm256_cmpgt_epi32(pcum, pi);
                    auto pmsk               = _mm256_movemask_ps(_mm256_castsi256_ps(pcmpgt));
                    auto psub               = _mm256_add_epi32(pcum, pcmpgt); // + -1 -> - 1
                    iofs                    = inc::index_of_first_set(pmsk);

                    if (iofs > 0){
                        i                  -= cur->offset[iofs - 1];
                    }

                    _mm256_storeu_si256((__m256i_u *)cur->offset, psub);
                    iofsg_ptr[0]            = iofs;
                    path_ptr[0]             = cur;
                    parent                  = cur;
                    iofsg_ptr              += 1;
                    path_ptr               += 1;
                    cur                     = cur->bottom[iofs];
                }
                else {
                    break;
                }
            }

            if (auto vals = unmark(cur); vals->talk_out(i, value) != is_empty){
                return;
            }
            else for(bool once = true; path_ptr != xref path[0]; ){
                iofsg_ptr                  -= 1;
                path_ptr                   -= 1;
                iofs                        = iofsg_ptr[0];
                parent                      = path_ptr[0];

                if (once){
                    once                    = false;
                    vals                    = unmark(parent->items[iofs]);
                    inc::free(vals);
                    parent->items[iofs]     = nullptr;
                }
                else{
                    inc::free(parent->bottom[iofs]);
                    parent->bottom[iofs]    = nullptr;
                }

                while(iofs < 7 and parent->bottom[iofs + 1] != nullptr){
                    inc::swap(xref parent->offset[iofs], xref parent->offset[iofs + 1]); 
                    inc::swap(xref parent->bottom[iofs], xref parent->bottom[iofs + 1]);
                    iofs                   += 1;
                }

                if (parent->bottom[0] != nullptr){
                    return;
                }
            }

            if (length() == 0){
                inc::free(root);
                root                        = null();
            }
        }

        item_t & operator[](uxx index){
            auto cur                        = (root);
            auto i                          = (i32)index;

            while(true){
                if (not is_item_level(cur)){
                    auto pi                 = _mm256_set1_epi32(i);
                    auto pcum               = _mm256_loadu_si256((__m256i_u *)cur->offset);
                    auto pcmpgt             = _mm256_cmpgt_epi32(pcum, pi);
                    auto pmsk               = _mm256_movemask_ps(_mm256_castsi256_ps(pcmpgt));
                    auto iofs               = inc::index_of_first_set(pmsk);

                    if (iofs > 0){
                        i                  -= cur->offset[iofs - 1];
                    }
                    cur                     = cur->bottom[iofs];
                }
                else{
                    auto vals               = (item_node *)unmark(cur);
                    return vals->get(i);
                }
            }
        }

        xpubgetx(length, uxx) {
            return root->offset[7];
        }
    $    
}

namespace mixc::docker_btree::origin{
    template<class final_t, class item_t>
    using btree = inc::adapter_array_access<
        mixc::docker_btree::btree<final_t, item_t>
    >;
}

#endif

#define xusing_docker_btree     ::mixc::docker_btree::origin
