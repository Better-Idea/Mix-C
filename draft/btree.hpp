#ifndef xpack_draft_btree
#define xpack_draft_btree
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_btree::inc
#include"instruction/index_of_first_set.hpp"
#include"macro/xalign.hpp"
#include"macro/xexport.hpp"
#include"math/const.hpp"
#include"meta/fit_bits.hpp"
#include"memop/swap.hpp"
#include"utils/bits_indexer.hpp"
#include"memory/allocator.hpp"
#include<intrin.h>
#pragma pop_macro("xuser")

namespace mixc::draft_btree::origin{
    using item_t = uxx;

    template<uxx bits, class item_t>
    struct xalign(16) node{
        enum{
            length      = 128 / bits,
        };

        inc::fit_bits<bits>         cum_offset[length] = {0};
        node<bits / 2, item_t>  *   items[length];
    };

    template<class item_t>
    struct xalign(16) node<4, item_t>{
        enum{
            length      = 64 / 4,
        };

        inc::bits_indexer<4>        cum_offset = {};
        item_t                      items[length];
    };

    template<class item_t>
    struct xalign(16) nodex{
        union{
            node<4, item_t>     *   items [4]   = {}; 
            nodex<item_t>       *   bottom[4]; 
        };
        i32                         offset[4]   = {};
    };

    struct btree{
        uxx                         count   = 0;
        uxx                         height  = 0;
        nodex<item_t>           *   root    = nullptr;

        using path_node                     = nodex<item_t>;
        using item_node                     = node<4, item_t>;

        void insert(uxx index, item_t const & value){
            if (node<4, item_t> * item_ptr; root == nullptr){
                root                        = alloc_path_node();
                item_ptr                    = alloc_item_node();
                root->items[0]              = item_ptr;
                height                      = 2;
            }

            nodex<item_t> * path[32];
            uxx  iofsg[32];
            auto path_ptr                   = path;
            auto iofsg_ptr                  = iofsg;
            auto h                          = height;
            auto i                          = (i32)index;
            auto ptr                        = root;
            auto iofs                       = uxx(0);
            auto new_node                   = (nodex<item_t> *)nullptr;
            auto parent                     = (nodex<item_t> *)nullptr;
            count                          += 1;

            while(true){
                if (h -= 1; h != 1){
                    auto pi                 = _mm_set1_epi32(i);
                    auto pcum               = _mm_load_si128((__m128i *)ptr->offset);
                    auto pcmplt             = _mm_cmplt_epi32(pi, pcum);
                    auto pcmpeq             = _mm_cmpeq_epi32(pi, pcum);
                    auto pmsk               = _mm_movemask_ps(_mm_castsi128_ps(pcmplt));
                    auto padd               = _mm_sub_epi32(pcum, pcmplt); // 减去 -1 -> +1

                    if (pmsk){
                        iofs                = inc::index_of_first_set(pmsk);
                    }
                    else{
                        // 找到 对应的子节点
                        // count:12 index:12
                        iofs                = _mm_movemask_ps(_mm_castsi128_ps(pcmpeq));
                        iofs                = inc::index_of_first_set(iofs);
                        padd                = _mm_sub_epi32(pcum, pcmpeq);
                    }
                    if (iofs > 0){
                        i                  -= ptr->offset[iofs - 1];
                    }

                    _mm_store_si128((__m128i *)ptr->offset, padd);
                    iofsg_ptr[0]            = iofs;
                    path_ptr[0]             = ptr;
                    iofsg_ptr              += 1;
                    path_ptr               += 1;
                    parent                  = path_ptr[-1];
                    ptr                     = ptr->bottom[iofs];
                }
                else{
                    i32  limit[]            = { 16, 32, 48, 64 };
                    auto i_node             = i >> 4;
                    auto i_offset           = i & 0xf;
                    auto parent             = ptr;
                    auto vals               = (node<4, item_t> *)ptr;
                    auto temp               = value;
                    iofsg_ptr[0]            = i_offset;
                    path_ptr[0]             = parent;
                    iofsg_ptr              += 1;
                    path_ptr               += 1;

                    for(bool can_move = true; i_node < 4; i_node++){
                        if (can_move){
                            if (parent->items[i_node] == nullptr){
                                parent->items[i_node]
                                            = alloc_item_node();
                            }
                            insert(i_offset, parent->items[i_node], temp, xref temp);
                            i_offset        = 0;
                        }

                        if (limit[i_node] == parent->offset[i_node]){
                            continue;
                        }

                        can_move            = false;
                        parent->offset[i_node]
                                           += 1;
                    }

                    if (parent->offset[3] < 16 * 4){
                        return;
                    }
                    else{
                        new_node            = alloc_path_node();
                        new_node->offset[0] = parent->offset[2] - parent->offset[1];
                        new_node->offset[1] = parent->offset[3] - parent->offset[1];
                        new_node->offset[2] = new_node->offset[1];
                        new_node->offset[3] = new_node->offset[1];
                        new_node->items[0]  = parent->items[2];
                        new_node->items[1]  = parent->items[3];
                        new_node->items[2]  = nullptr;
                        new_node->items[3]  = nullptr;
                        parent->items[2]    = nullptr;
                        parent->items[3]    = nullptr;
                        parent->offset[2]   = parent->offset[1];
                        parent->offset[3]   = parent->offset[1];
                        break;
                    }
                }
            }

            while(true){
                iofsg_ptr                  -= 1;
                path_ptr                   -= 1;

                if (path_ptr == xref path[0]){
                    height                 += 1;
                    break;
                }

                iofs                        = iofsg_ptr[-1];
                parent                      = path_ptr[-1];
                auto offset                 = path_ptr[0]->offset[3] + (iofs ? parent->offset[iofs - 1] : 0);
                auto need_return            = parent->bottom[3] == nullptr;
                auto node_temp              = new_node;

                for(;; iofs += 1){
                    if (inc::swap(xref parent->offset[iofs], xref offset); iofs == 3){
                        break;
                    }
                    inc::swap(xref parent->bottom[iofs + 1], xref node_temp);
                }
                if (need_return){
                    return;
                }

                new_node                    = alloc_path_node();

                // 转移节点
                new_node->bottom[0]         = parent->bottom[2];
                new_node->bottom[1]         = parent->bottom[3];
                new_node->bottom[2]         = node_temp;
                new_node->bottom[3]         = nullptr;
                parent->bottom[2]           = nullptr;
                parent->bottom[3]           = nullptr;

                // 转移偏移
                new_node->offset[0]         = parent->offset[2] - parent->offset[1];
                new_node->offset[1]         = parent->offset[3] - parent->offset[1];
                new_node->offset[2]         = new_node->offset[1] + node_temp->offset[3];
                new_node->offset[3]         = new_node->offset[2];
                parent->offset[2]           = parent->offset[1];
                parent->offset[3]           = parent->offset[1];
            }

            auto new_root                   = alloc_path_node();
            new_root->bottom[0]             = root;
            new_root->bottom[1]             = new_node;
            new_root->bottom[2]             = nullptr;
            new_root->bottom[3]             = nullptr;
            new_root->offset[0]             = root->offset[3];
            new_root->offset[1]             = root->offset[3] + new_node->offset[3];
            new_root->offset[2]             = new_root->offset[1];
            new_root->offset[3]             = new_root->offset[1];
            root                            = new_root;
        }

        item_t & operator[](uxx index){
            auto h                          = height;
            auto ptr                        = root;
            auto i                          = (i32)index;

            while(true){
                if (h -= 1; h != 0){
                    auto pi                 = _mm_set1_epi32(i);
                    auto pcum               = _mm_load_si128((__m128i *)ptr->offset);
                    auto pcmplt             = _mm_cmplt_epi32(pi, pcum);
                    auto pmsk               = _mm_movemask_ps(_mm_castsi128_ps(pcmplt));
                    auto iofs               = inc::index_of_first_set(pmsk);
                    // xhint(iofs, h, i);

                    if (iofs > 0){
                        i                  -= ptr->offset[iofs - 1];
                    }
                    ptr                     = ptr->bottom[iofs];
                }
                else{
                    auto vals               = (node<4, item_t> *)ptr;
                    auto idx                = vals->cum_offset.get(i);
                    return vals->items[idx];
                }
            }
        }

        uxx insert(uxx index, node<4, item_t> * vals, item_t const & value, item_t * last = nullptr){
            uxx i_pop                       = vals->cum_offset.get(15/*last*/);
            // xhint(i_pop);
            if (last == nullptr){
                vals->items[i_pop]          = value;
            }
            else{
                inc::swap(xref vals->items[i_pop], xref last[0]);
            }
            return vals->cum_offset.insert(index, i_pop);
        }

        item_node * alloc_item_node(){
            return inc::alloc_with_initial<item_node>(
                inc::memory_size{sizeof(item_node)}
            );
        }

        path_node * alloc_path_node(){
            return inc::alloc_with_initial<path_node>(
                inc::memory_size{sizeof(path_node)}
            );
        }
    };
}

#endif

xexport_space(mixc::draft_btree::origin)
