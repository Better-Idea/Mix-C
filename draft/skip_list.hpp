#define xuser mixc::powerful_cat
#include"define/base_type.hpp"
#include"memop/swap.hpp"

namespace xuser{
    struct skip_node;

    using item_t = uxx;

    enum skip_node_state{
        need_delete,
        is_narrow_space,    // 与下一个跳跃的节点相隔 0 个节点
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
            state(need_delete),
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
        uxx         top_step = 1;

        static constexpr bool path_record = true;

        template<auto path_record = false>
        auto find(uxx * index_ptr, skip_node ** path_ptr = nullptr){
            skip_node * cur_skip_node = root;
            item_node * item_ptr;
            uxx       & i             = index_ptr[0];

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
                if (cur_skip_node->skip_step += 1; cur_skip_node->is_item){
                    break;
                }
                // 如果越不过，那么该跳跃点至下一个跳跃点之间多了一个节点，而插入点也在此范围内
                else{
                    if constexpr (path_record){
                        path_ptr[0]             = cur_skip_node;
                        path_ptr               += 1;
                    }
                    cur_skip_node               = cur_skip_node->bottom;
                }
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
            skip_node ** path_ptr               = path + 1/*skip top*/;
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
            auto pair                           = find<path_record>(xref i, path_ptr);
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
                    new_skip_node                   = alloc_skip_node(cur_skip_node);
                    cur_skip_node->state            = is_normal_space;

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

                if (cur_skip_node == root){
                    break;
                }

                // 若为 true 说明已经在 root 层插入了新节点
                if (path_ptr == xref path[1]){
                    if (top_step += 1; top_step != is_wide_space){
                        return;
                    }
                    else{
                        top_step                    = is_narrow_space;
                    }

                    new_skip_node                   = alloc_skip_node();
                    new_skip_node->next             = nullptr;

                    // 平衡跳表的 if 判定要求 state 为未插入元素时的状态
                    new_skip_node->state            = 0;

                    // 顶层的 skip_step 不可以直接赋值为 count
                    // 因为每一层都可能存在不足一个跳跃点的末尾部分
                    new_skip_node->skip_step        = 0;
                    new_skip_node->is_item          = false;
                    new_skip_node->bottom           = root;

                    while(root->next != nullptr){
                        new_skip_node->state       += 1;
                        root                        = root->next;
                    }

                    root                            = new_skip_node;
                    cur_skip_node                   = new_skip_node;
                }
                else{
                    path_ptr                       -= 1;
                    cur_skip_node                   = path_ptr[0];
                }
            }
        }

        item_t & operator[](uxx index){
            item_node * item = find(xref index).item_ptr;
            return item->value;
        }

        loop_t when_normal_space(skip_node * cur_skip_node, skip_node * nex_skip_node){
           /* 下一个跳跃点为 NULL 直接跳过：
            * 插入前状态：
            * [S1]========NULL
            *  || 
            * [N0]==[N1]==NULL
            * 
            * 或者：
            * [S1]========[S?]==···
            *  ||          ||
            * [N0]==[N1]==[N1]==···
            * 
            * 插入后状态：
            * [S3]==============NULL
            *  || 
            * [N0]==[N1]==[N2]==NULL
            * 
            * 或者：
            * [S3]==============[S?]==···
            *  ||                ||
            * [N0]==[N1]==[N2]==[N3]==···
            */

            skip_node * new_skip_node;

            if (nex_skip_node == nullptr){
                new_skip_node                   = alloc_skip_node(cur_skip_node);
                new_skip_node->state            = is_narrow_space;

                if (not cur_skip_node->is_item){
                    new_skip_node->skip_step    = cur_skip_node->bottom->next->next->skip_step;
                    cur_skip_node->skip_step   -= new_skip_node->skip_step; // 分给 new_skip_node
                }
                else{
                    new_skip_node->skip_step    = is_narrow_space;
                    cur_skip_node->skip_step    = is_normal_space;
                }
                return loop_t::go_on;
            }

           /* 注意：
            * 我们通常是以当前插入点的下一个跳跃点作为观察对象（不会考察前一个跳跃点）
            * 看两个跳跃点之间是否可以合并或者分裂
            * 
            * 此外：
            * 一宽一窄，可以平衡一下：
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
            */
            if (nex_skip_node->state == is_narrow_space){
                cur_skip_node->state            = is_normal_space;
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
            if (nex_skip_node->state != is_wide_space){
                // next->state == is_normal_space
                return loop_t::finish;
            }

            new_skip_node                       = alloc_skip_node(cur_skip_node);
            cur_skip_node->state                = is_normal_space;
            nex_skip_node->state                = is_normal_space;

            if (not cur_skip_node->is_item){
                uxx s0                          = cur_skip_node->skip_step;
                uxx s1                          = nex_skip_node->bottom->skip_step;
                uxx s2                          = 
                    cur_skip_node->bottom->skip_step + 
                    cur_skip_node->bottom->next->skip_step;
                uxx s3                          = s0 + s1 - s2;

                cur_skip_node->skip_step        = s2;
                new_skip_node->skip_step        = s3;
                nex_skip_node->skip_step       -= s1;
            }
            else{
                cur_skip_node->skip_step        = is_normal_space;
                new_skip_node->skip_step        = is_normal_space;
                nex_skip_node->skip_step        = is_normal_space;
            }

            cur_skip_node->next                 = new_skip_node;
            nex_skip_node->bottom               = nex_skip_node->bottom->next;
            return loop_t::go_on;
        }

        skip_node * alloc_skip_node(skip_node * cur_skip_node){
            skip_node * 
            nex_skip_node                   = cur_skip_node->next;
            skip_node *
            new_skip_node                   = alloc_skip_node();
            new_skip_node->next             = nex_skip_node;
            new_skip_node->state            = is_normal_space;
            new_skip_node->state            = is_normal_space;
            new_skip_node->is_item          = cur_skip_node->is_item;
            new_skip_node->bottom           = cur_skip_node->bottom->next->next;
            cur_skip_node->next             = new_skip_node;
            return new_skip_node;
        }

        skip_node * alloc_skip_node(){
            return new skip_node();
        }

        item_node * alloc_item_node(item_t const & value){
            return new item_node(value);
        }
    };
}
