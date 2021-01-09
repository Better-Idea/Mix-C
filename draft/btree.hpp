#define xuser mixc::powerful_cat
#include"meta/fit_bits.hpp"
#include"utils/bits_indexer.hpp"
#include"math/const.hpp"
#include"instruction/index_of_first_set.hpp"
#include<intrin.h>

namespace xuser{
    using item_t = uxx;

    template<uxx bits, class item_t>
    struct node{
        enum{
            length      = 128 / bits,
        };

        fit_bits<bits>              cum_offset[length] = {0};
        node<bits / 2, item_t>  *   items[length];
    };

    template<class item_t>
    struct node<4, item_t>{
        enum{
            length      = 64 / 4,
        };

        bits_indexer<4>             cum_offset = {};
        item_t                      items[length];
    };

    template<class item_t>
    struct nodex{
        union{
            node<4, item_t>     *   items [4]    = {}; 
            nodex<item_t>       *   bottom[4]; 
        };
        u32                         offset[4]   = {};
        u32                         mask        = 0;
    };

    struct tree{
        uxx                         count   = 0;
        uxx                         height  = 0;
        nodex<item_t>           *   root    = nullptr;

        void insert(uxx index, item_t const & value){
            if (node<4, item_t> * item_ptr; root == nullptr){
                item_ptr            = new node<4, item_t>;
                root                = new nodex<item_t>;
                root->items[0]      = item_ptr;
                item_ptr->items[0]  = value;
                count               = 1;
                height              = 2;
                item_ptr->cum_offset.insert(0, 0);
                _mm_store_si128(root->offset, _mm_set1_epi32(1 + min_value_of<i32>));
                return;
            }

            nodex<item_t> * path[32];
            uxx  iofsg[32];
            auto path_ptr           = path;
            auto iofsg_ptr          = iofsg;
            auto h                  = height;
            auto i                  = index;
            auto ptr                = root;
            auto iofs               = uxx(0);
            count                  += 1;

            while(true){
                if (h -= 1; h != 0){
                    auto pi         = _mm_set1_epi32(i + min_value_of<i32>);
                    auto pcum       = _mm_load_si128((__m128i *)ptr->offset);
                    auto pcmp       = _mm_cmpgt_epi32(pcum, pi);
                    auto pmsk       = _mm_movemask_ps(_mm_castsi128_ps(pcmp));
                    auto padd       = _mm_sub_epi32(pcum, pmsk); // 减去 -1 -> +1
                    iofs            = index_of_first_set(pmsk);
                    iofsg_ptr[0]    = iofs;
                    path_ptr[0]     = ptr;
                    iofsg_ptr      += 1;
                    path_ptr       += 1;
                    ptr             = ptr->bottom[iofs];
                    i              -= ptr->offset[iofs];
                    _mm_store_epi64(ptr->offset, padd);
                }
                else{
                    auto plimit     = _mm_setr_epi32(
                        16 * 1 + 1, 
                        16 * 2 + 1, 
                        16 * 3 + 1, 
                        16 * 4 + 1
                    );
                    auto pcum       = _mm_load_si128((__m128i *)path_ptr[-1]->offset);
                    auto pof        = _mm_cmpeq_epi32(plimit, pcum); // parallel overflow
                    auto pofp       = u32p(xref pof);
                    auto psub       = _mm_add_pi32(pcum, pof);       // 饱和
                    auto i_pop      = uxx(0);
                    auto i0         = uxx(0);
                    auto vals       = (node<4, item_t> *)ptr;

                    if (ptr == nullptr){
                        vals        = path_ptr[-1]->items[iofs] = new node<4, item_t>;
                    }
                    _mm_store_epi64(path_ptr[-1]->offset, psub);

                    // 未存满
                    if (~pofp[iofs] == 0){
                        i_pop               = vals->cum_offset.get(15/*last*/);
                        vals->items[i_pop]  = values;
                        vals->cum_offset.insert(i, i_pop);
                        return;
                    }

                    // 存满
                    if (iofs == 0){
                        
                    }
                    if (iofs == 3){

                    }
                }
            }
        }

        void move(node<4, item_t> * vals, ){

        }
    };
}
