#ifndef xpack_draft_btree
#define xpack_draft_btree
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_btree::inc
#include"dumb/mirror.hpp"
#include"instruction/index_of_first_set.hpp"
#include"macro/xalign.hpp"
#include"macro/xexport.hpp"
#include"memop/swap.hpp"
#include"memory/allocator.hpp"
#include"utils/bits_indexer.hpp"
#include<intrin.h>
#pragma pop_macro("xuser")

namespace mixc::draft_btree::origin{
    using item_t = uxx;

    constexpr bool is_full   = true;

    template<class item_t>
    struct item_group{
        enum{ 
            length = 16, 
            group_max = 4
        };

        struct xalign(16) item_node{
            inc::bits_indexer<4>        cum_offset;
            item_t                      item[length];
        };

        item_node * groups[group_max]       = {};
        uxx         count                   = 0;

        bool insert(uxx i, item_t const & value){
            using m_t = inc::mirror<item_t>;

            // 带构造的初始化一次，但 m 析构时不析构 item_t
            m_t         m                   = { value, inc::construction_t::execute };
            uxx         i_group             = i >> 4;
            uxx         i_group_max         = count >> 4;
            uxx         i_offset            = i & 0xf;
            uxx         i_free;
            item_node * group;


            for(uxx i = i_group; i <= i_group_max; i++){
                if (groups[i] == nullptr){
                    groups[i]               = inc::alloc_with_initial<item_node>(
                        inc::memory_size{sizeof(item_node)}
                    );
                    group                   = groups[i];
                    i                       = group_max;
                }
                else{
                    group                   = groups[i];
                }

                i_free                      = group->cum_offset.get(15/*最后一个*/);
                inc::swap(xref m, (m_t *)(xref group->item[i_free]));
                group->cum_offset.insert(i_offset, i_free);
                i_offset                    = 0;
            }

            // full
            count                          += 1;
            return count == group_max * length;
        }

        item_group * split(){
            item_group<item_t> * brother    = inc::alloc_with_initial<item_group>(
                inc::memory_size{sizeof(item_group<item_t>)}
            );

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
            uxx         i_real_offset       = group->cum_offset.get(i_offset);
            return group->item[i_real_offset];
        }
    };

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

    public:
        uxx             count               = 0;
        uxx             height              = 0;
        path_node   *   root                = nullptr;

        void insert(uxx index, item_t const & value){
            if (item_node * item_ptr; root == nullptr){
                root                        = alloc_path_node();
                item_ptr                    = alloc_item_node();
                root->items[0]              = item_ptr;
                height                      = 2;
            }

            path_node * path[32];
            uxx  iofsg[32];
            auto path_ptr                   = path;
            auto iofsg_ptr                  = iofsg;
            auto h                          = height;
            auto i                          = (i32)index;
            auto cur                        = root;
            auto iofs                       = uxx(0);
            auto new_node                   = (path_node *)nullptr;
            auto new_item                   = (item_node *)nullptr;
            auto parent                     = (path_node *)nullptr;
            auto offset                     = 0;
            count                          += 1;

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
                        // 找到 对应的子节点
                        // count:12 index:12
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

        item_t & operator[](uxx index){
            auto h                          = height;
            auto cur                        = root;
            auto i                          = (i32)index;

            while(true){
                if (h -= 1; h != 0){
                    auto pi                 = _mm256_set1_epi32(i);
                    auto pcum               = _mm256_loadu_si256((__m256i *)cur->offset);
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
            // TODO: inc::alloc_with_initial暂时还不支持超过 16 字节的对齐内存分配
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
    };
}

#endif

xexport_space(mixc::draft_btree::origin)
