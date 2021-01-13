#ifndef xpack_draft_btree
#define xpack_draft_btree
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_btree::inc
#include"dumb/mirror.hpp"
#include"dumb/move.hpp"
#include"instruction/index_of_first_set.hpp"
#include"macro/xalign.hpp"
#include"macro/xexport.hpp"
#include"memop/swap.hpp"
#include"memory/allocator.hpp"
#include"utils/bits_indexer.hpp"
#include<intrin.h>
#pragma pop_macro("xuser")

namespace mixc::draft_btree::origin{
    constexpr bool is_full  = true;
    constexpr bool is_empty = true;

    template<class item_t>
    struct item_group{
        enum{ 
            length          = 16, 
            group_max       = 4
        };

        using m_t           = inc::mirror<item_t>;

        struct xalign(16) item_node{
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

            // 当 count 值为 16, 32, 48 时需要释放最后一个组，因为通过该函数带出一个元素后
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

            // 移动语义
            value[0]                        =  inc::move((item_t &)m);

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
    };

    static inline const i32 null_node = 0;

    template<class item_t>
    struct btree{
    private:
        using item_node         = item_group<item_t>;

        struct xalign(32) path_node{
            union{
                item_node             * items [8]   = {}; 
                path_node             * bottom[8];
            };
            i32                         offset[8]   = {};
        };

        static path_node * null(){
            // 用 null() 代替 nullptr，让 btree::length() 可以在只有 0 个元素的时候可访问
            uxx     offset                  = uxx(& ((path_node *)nullptr)->offset[7]);
            u08p    ptr                     = u08p(& null_node) - offset;
            return (path_node *)ptr;
        }
    public:
        uxx             height              = 0;
        path_node   *   root                = null();

        void insert(uxx index, item_t const & value){
            if (item_node * item_ptr; root == null()){
                root                        = alloc_path_node();
                item_ptr                    = alloc_item_node();
                root->items[0]              = item_ptr;
                height                      = 2;
            }

            // index of first set group
            uxx         iofsg[32];
            path_node * path[32];
            auto path_ptr                   = (path);
            auto iofsg_ptr                  = (iofsg);
            auto h                          = (height);
            auto i                          = (i32)index;
            auto cur                        = (root);
            auto iofs                       = (uxx)0;
            auto new_node                   = (path_node *)nullptr;
            auto new_item                   = (item_node *)nullptr;
            auto parent                     = (path_node *)nullptr;
            auto offset                     = (0);

            while(true){
                if (h -= 1; h != 0){
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

                    _mm256_storeu_si256((__m256i_u *)cur->offset, padd);
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

            if (auto vals = (item_node *)cur; vals->insert(i, value) != is_full){
                return;
            }
            else{
                new_item                    = vals->split();
                new_node                    = (path_node *)new_item;
                offset                      = new_item->count;
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
            height                         += 1;
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
            auto h                          = (height);
            auto cur                        = (root);
            auto parent                     = (root);
            auto i                          = (i32)index;
            auto iofs                       = (0);

            while(true){
                if (h -= 1; h != 0){
                    auto pi                 = _mm256_set1_epi32(i);
                    auto pcum               = _mm256_loadu_si256((__m256i_u *)cur->offset);
                    auto pcmpgt             = _mm256_cmpgt_epi32(pcum, pi);
                    auto pmsk               = _mm256_movemask_ps(_mm256_castsi256_ps(pcmpgt));
                    auto psub               = _mm256_add_epi32(pcum, pcmpgt); // + -1 -> -1
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

            if (auto vals = (item_node *)cur; vals->talk_out(i, value) != is_empty){
                return;
            }

            for(bool once = true; path_ptr != xref path[0]; ){
                iofsg_ptr                  -= 1;
                path_ptr                   -= 1;
                iofs                        = iofsg_ptr[0];
                parent                      = path_ptr[0];

                if (once){
                    once                    = false;
                    free(parent->items[iofs]);
                }
                else{
                    free(parent->bottom[iofs]);
                }
                parent->bottom[iofs]        = nullptr;

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
                free(root);
                root                        = null();
            }
        }

        uxx length() const {
            return root->offset[7];
        }

        item_t & operator[](uxx index){
            auto h                          = (height);
            auto cur                        = (root);
            auto i                          = (i32)index;

            while(true){
                if (h -= 1; h != 0){
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
                    auto vals               = (item_node *)cur;
                    return vals->get(i);
                }
            }
        }

        item_node * alloc_item_node(){
            // TODO: inc::alloc_with_initial 暂时还不支持超过 16 字节的对齐内存分配
            // 目前该接口使用 AVX 在 5M 数据随机插入性能 大约有 7% 的性能差距
            // 测试       ：5M 数据随机插入 | 读取
            // 32Byte 对齐：提升约 7%       | 提升约 7%
            return inc::alloc_with_initial<item_node>(
                inc::memory_size{sizeof(item_node)}
            );
            // return new item_node;
        }

        path_node * alloc_path_node(){
            return inc::alloc_with_initial<path_node>(
                inc::memory_size{sizeof(path_node)}
            );
            // return new path_node;
        }

        template<class node_t>
        void free(node_t * ptr){
            inc::free(ptr, inc::memory_size{sizeof(node_t)});
        }
    };
}

#endif

xexport_space(mixc::draft_btree::origin)
