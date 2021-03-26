// TODO：用于平衡的部分还需要重新设置父节点的跳跃数
#define xuser mixc::powerful_cat
#include"define/base_type.hpp"
#include"memop/swap.hpp"
#include"macro/xnew.hpp"
#include"utils/allocator.hpp"

namespace xuser{
    struct skip_node;

    using item_t = uxx;

    enum skip_node_state{
        is_narrow_space = 1,// 与下一个跳跃的节点相隔 0 个节点
        is_normal_space,    // 与下一个跳跃的节点相隔 1 个节点
        is_wide_space,      // 与下一个跳跃的节点相隔 2 个节点
        need_split,
    };

    struct tmpl_next{
        tmpl_next       * next;
    };

    struct item_node{
        item_node       * next;
        item_t            value;

        item_node(item_t const & value) : 
            next(nullptr),
            value(value){
        }
    };

    struct skip_node{
        skip_node       * next;
        uxx               skip_step : sizeof(uxx) * 8 - 4;
        uxx               state     : 3;
        uxx               is_item   : 1;

        skip_node():
            next(nullptr),
            skip_step(0),
            state(is_narrow_space),
            is_item(false),
            item(nullptr){
        }

        union{
            item_node   * item;
            skip_node   * bottom;
        };
    };

    struct skip_list{
        skip_node * root     = nullptr;
        uxx         count    = 0;
        uxx         state    = is_narrow_space;

        static constexpr bool path_record = true;

        template<auto path_record = false>
        auto find(uxx * index_ptr, skip_node *** path_ptrx = nullptr){
            item_node *   item_ptr;
            skip_node *   cur_skip_node         = root;
            skip_node **& path_ptr              = path_ptrx[0];
            uxx         & i                     = index_ptr[0];
            uxx           index                 = i;

            while(true){
               /*
                * [S9]==================================================[S1]==NULL
                *  ||                                                    ||
                * [S4]====================[S5]==========================[S1]==NULL
                *  ||                      ||                            ||
                * [S2]========[S2]========[S3]==============[S2]========[S1]==NULL
                *  ||          ||          ||                ||          ||
                * [N0]==[N1]==[N2]==[N3]==[N4]==[N5]==[N6]==[N7]==[N8]==[N9]==[NA]
                */ 
                while(cur_skip_node->skip_step <= i and cur_skip_node->next != nullptr){
                    i                          -= cur_skip_node->skip_step;
                    cur_skip_node               = cur_skip_node->next;
                }

                // 底层节点是元素节点
                if constexpr (path_record){
                    path_ptr[0]                 = cur_skip_node;
                    path_ptr                   += 1;
                    cur_skip_node->skip_step   += 1;
                }
                if (cur_skip_node->is_item){
                    break;
                }
                // 如果越不过，那么该跳跃点至下一个跳跃点之间多了一个节点，而插入点也在此范围内
                cur_skip_node                   = cur_skip_node->bottom;
            }

            for(item_ptr = cur_skip_node->item;
                item_ptr->next != nullptr and i != 0; 
                i -= 1){
                item_ptr                        = item_ptr->next;
            }

            struct pair{ item_node * item_ptr; skip_node * cur_skip_node; };
            return pair{ item_ptr, cur_skip_node };
        }

        void insert(uxx index, item_t const & value){
            item_node *  insert_point;
            item_node *  new_item               = alloc_item_node(value);
            skip_node *  cur_skip_node;
            skip_node *  new_skip_node;
            skip_node *  nex_skip_node;
            skip_node *  path[sizeof(uxx) * 8];
            skip_node ** path_ptr               = path;
            uxx          i                      = index;

            if (count += 1; root == nullptr){
                root                            = alloc_skip_node();
                root->next                      = nullptr;
                root->skip_step                 = is_narrow_space;
                root->state                     = is_narrow_space;
                root->is_item                   = true;
                root->item                      = new_item;
                return;
            }

            // 说明是插入到末尾(相当于追加，那么 i 一定等于 1)
            auto pair                           = find<path_record>(xref i, xref path_ptr);
            insert_point                        = pair.item_ptr;
            cur_skip_node                       = pair.cur_skip_node;

            if (i == 0){
                swap(xref insert_point->value, xref new_item->value);
                new_item->next                  = insert_point->next;
            }

            insert_point->next                  = new_item;

            while(true){
                if (nex_skip_node = cur_skip_node->next;
                    cur_skip_node->state == is_narrow_space){
                   /* 下一个跳跃点为 NULL 直接跳过：
                    * 插入前状态：
                    * [S1]==NULL
                    *  || 
                    * [N0]==[N1]
                    * 
                    * 或者：
                    * [S1]==[S?]==···
                    *  ||    ||
                    * [N0]==[N1]==···
                    * 
                    * 插入后状态：
                    * [S2]========NULL
                    *  ||          
                    * [N0]==[N1]==NULL
                    *  ^^
                    *  ||
                    *  新增的
                    * 
                    * 或者：
                    * [S2]========[S?]==···
                    *  ||          ||
                    * [N0]==[N1]==[N2]==···
                    *  ^^
                    *  ||
                    *  新增的
                    * 直接跳过就好了
                    */
                    cur_skip_node->state           += 1;
                    return;
                }
                else if (cur_skip_node->state == is_normal_space){
                    if (when_normal_space(cur_skip_node, nex_skip_node) == loop_t::finish){
                        return;
                    }
                }
                else {
                   /* 插入前状态：
                    * [S3]==============[S?]==···
                    *  ||                ||
                    * [N0]==[N1]==[N2]==[N3]==···
                    * 
                    * 插入后：
                    * [S4]====================[S?]==···
                    *  ||                      ||
                    * [N0]==[N1]==[N2]==[N3]==[N4]==···
                    *              ^^
                    *              ||
                    *              新增的
                    *
                    * 可以自行分裂：
                    * [S2]========[S2]========[S?]==
                    *  ||          ||          ||   
                    * [N0]==[N1]==[N2]==[N3]==[N4]==
                    */
                    new_skip_node                   = alloc_skip_node();
                    new_skip_node->next             = nex_skip_node;
                    cur_skip_node->next             = new_skip_node;
                    new_skip_node->state            = is_normal_space;
                    cur_skip_node->state            = is_normal_space;
                    new_skip_node->is_item          = cur_skip_node->is_item;
                    new_skip_node->bottom           = cur_skip_node->bottom->next->next;

                    if (not cur_skip_node->is_item){
                        uxx s0                      = cur_skip_node->skip_step;
                        uxx s1                      = 
                            cur_skip_node->bottom->skip_step + 
                            cur_skip_node->bottom->next->skip_step;
                        uxx s3                      = s0 - s1;

                        cur_skip_node->skip_step    = s1;
                        new_skip_node->skip_step    = s3;
                    }
                    else{
                        cur_skip_node->skip_step    = is_normal_space;
                        new_skip_node->skip_step    = is_normal_space;
                    }
                }

                // 若为 true 说明已经在 root 层插入了新节点
                if (path_ptr -= 1; path_ptr < xref path[0]){
                    return;
                }
                if (path_ptr == xref path[0]){
                    if (state != is_normal_space){
                        state                      += 1;
                        return;
                    }

                    new_skip_node                   = alloc_skip_node();
                    nex_skip_node                   = alloc_skip_node();
                    new_skip_node->next             = nex_skip_node;
                    nex_skip_node->next             = nullptr;
                    new_skip_node->skip_step        = root->skip_step + root->next->skip_step;
                    nex_skip_node->skip_step        = root->next->next->skip_step;
                    new_skip_node->state            = is_normal_space;
                    nex_skip_node->state            = is_narrow_space;
                    new_skip_node->is_item          = false;
                    nex_skip_node->is_item          = false;
                    new_skip_node->bottom           = root;
                    nex_skip_node->bottom           = root->next->next;
                    root                            = new_skip_node;
                    return;
                }
                else{
                    cur_skip_node                   = path_ptr[0];
                }
            }
        }

        item_t & operator[](uxx index){
            item_node * item = find(xref index).item_ptr;
            return item->value;
        }

        loop_t when_normal_space(skip_node * cur_skip_node, skip_node * nex_skip_node){
           /* 插入前状态：
            * [S2]========NULL
            *  || 
            * [N0]==[N1]==NULL
            * 
            * 插入后状态：
            * [S2]========[S1]==NULL
            *  ||          ||
            * [N0]==[N1]==[N2]==NULL
            */

            if (nex_skip_node == nullptr){
                nex_skip_node                   = alloc_skip_node();
                nex_skip_node->next             = nullptr;
                nex_skip_node->state            = is_narrow_space;
                nex_skip_node->is_item          = cur_skip_node->is_item;
                nex_skip_node->bottom           = cur_skip_node->bottom->next->next;
                cur_skip_node->next             = nex_skip_node;

                if (not cur_skip_node->is_item){
                    nex_skip_node->skip_step    = cur_skip_node->bottom->next->next->skip_step;
                    cur_skip_node->skip_step   -= nex_skip_node->skip_step; // 分给 nex_skip_node
                }
                else{
                    nex_skip_node->skip_step    = is_narrow_space;
                    cur_skip_node->skip_step    = is_normal_space;
                }
                return loop_t::go_on;
            }

           /* 注意：
            * 我们通常是以当前插入点的下一个跳跃点作为观察对象（不会考察前一个跳跃点）
            * 看两个跳跃点之间是否可以合并或者分裂
            * 
            * 此外：
            * 插入后，一宽一窄，可以平衡一下：
            * [S3]==============[S1]==[S?]==···
            *  ||                ||    ||
            * [N0]==[N1]==[N2]==[N3]==[N4]==···
            *        ^^
            *        ||
            *        新增
            * 
            * 如下情况也是类似的：
            * [S3]==============[S1]==NULL
            *  ||                ||
            * [N0]==[N1]==[N2]==[N3]==NULL
            *        ^^
            *        ||
            *        新增
            * 
            * 做如下变化：
            * [S2]========[S2]========[S?]==···
            *  ||          ||          ||
            * [N0]==[N1]==[N2]==[N3]==[N4]==···
            * 
            * 或者：
            * [S2]========[S2]==NULL
            *  ||          ||
            * [N0]==[N1]==[N2]==[N3]==NULL
            */
            if (nex_skip_node->state == is_narrow_space){
                nex_skip_node->state            = is_normal_space;

                if (not cur_skip_node->is_item){
                    uxx s0                      = cur_skip_node->skip_step;
                    uxx s1                      = 
                        cur_skip_node->bottom->skip_step + 
                        cur_skip_node->bottom->next->skip_step;
                    uxx s2                      = nex_skip_node->skip_step;
                    cur_skip_node->skip_step    = s1;
                    nex_skip_node->skip_step    = s0 - s1 + s2;
                }
                else{
                    cur_skip_node->skip_step    = is_normal_space;
                    nex_skip_node->skip_step    = is_normal_space;
                }

                nex_skip_node->bottom           = cur_skip_node->bottom->next->next;
                return loop_t::finish;
            }

           /* 两宽，可以分裂：
            * [S3]==============[S3]==============[S?]==···
            *  ||                ||                || 
            * [N0]==[N1]==[N2]==[N3]==[N4]==[N5]==[N6]==···
            *        ^^
            *        ||
            *        插入点
            * 
            * 做如下变化：
            * [S2]========[S2]========[S2]========[S?]==···
            *  ||          ||          ||          || 
            * [N0]==[N1]==[N2]==[N3]==[N4]==[N5]==[N6]==···
            */
            if (nex_skip_node->state < is_wide_space){
                cur_skip_node->state           += 1;
                return loop_t::finish;
            }

            skip_node * new_skip_node           = alloc_skip_node();
            new_skip_node->next                 = nex_skip_node;
            cur_skip_node->next                 = new_skip_node;
            new_skip_node->state                = is_normal_space;
            cur_skip_node->state                = is_normal_space;
            new_skip_node->is_item              = cur_skip_node->is_item;

            if (not cur_skip_node->is_item){
                uxx s0                          = cur_skip_node->skip_step;
                uxx s1                          = nex_skip_node->bottom->skip_step;
                uxx s2                          =
                    cur_skip_node->bottom->skip_step + 
                    cur_skip_node->bottom->next->skip_step;
                uxx s3                          = s0 + s1 - s2; // 后两个子节点 skip_count 总和

                cur_skip_node->skip_step        = s2;
                new_skip_node->skip_step        = s3;
                nex_skip_node->skip_step       -= s1;
            }
            else{
                cur_skip_node->skip_step        = is_normal_space;
                new_skip_node->skip_step        = is_normal_space;
                nex_skip_node->skip_step        = is_normal_space;
            }

            new_skip_node->bottom               = cur_skip_node->bottom->next->next;
            nex_skip_node->bottom               = nex_skip_node->bottom->next;
            return loop_t::go_on;
        }

        skip_node * alloc_skip_node(){
            return alloc_with_initial<skip_node>(
                memory_size{sizeof(skip_node)}
            );
        }

        item_node * alloc_item_node(item_t const & value){
            return alloc_with_initial<item_node>(
                memory_size{sizeof(item_node)}, value
            );
        }
    };
}
