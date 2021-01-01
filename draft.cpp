#define xuser mixc::powerful_cat
// // #include"docker/shared.hpp"
// #include"define/base_type.hpp"

// using item_t = uxx;

// struct node;

// uxx     null_node       = 0;
// node *  null_node_ptr   = (node *) & null_node;

// typedef struct ordered_node{
//     ordered_node *  prev;
//     ordered_node *  next;
//     item_t          value;
// } onode;

// struct tree{
//     onode * get_onode(uxx i){

//     }

//     onode * append_onode(){

//     }

//     uxx capacity(){

//     }

//     uxx count(){

//     }

//     uxx height(){

//     }

//     uxx & get_offset(uxx lv, uxx i){

//     }

//     void insert(uxx index){

//         // 先创建新节点，让元素总数 +1
//         onode * new_item    = append_onode();

//         uxx i               = index;
//         uxx j               = 0;
//         uxx cnt             = count();
//         uxx lv              = height();
//         uxx half            = capacity() / 2;
//         uxx offset;

//         if (cnt == 1){
//             new_item->prev = new_item;
//             new_item->next = new_item;
//             return;
//         }

//         while(lv-- > 1){
//             offset          = get_offset(lv, j);
//             i               = i < offset ? cnt - offset + i : offset - i;

//             // 插入左边
//             if (i < half){
//                 j         <<= 1;
//             }
//             // 插入右边
//             else{
//                 j         <<= 1;
//                 j          += 1;
//                 i          -= half;
//             }
//         }

//         if (){

//         }
//     }
// };

#include"math/random.hpp"
#include"io/tty.hpp"

namespace xuser{
    void test(){
        for(uxx i = 1; i <= 16; i++){
            u32 v = random<u32>() % i;
            tty.write_line(v);
        }
    }
}

/*

uxx     i_node                  = 0;
uxx  *  offset_ptr              = nullptr;
node *  tmp;
node *  new_node;

iteration_by(index, [&](uxx arg_lv, uxx arg_i_node, uxx * arg_offset_ptr){
    if (arg_lv != 0){
        return;
    }
    i_node                      = arg_i_node;
    offset_ptr                  = arg_offset_ptr;
});

// 插入到右节点
// lv0 的 offset_ptr[0] 不是 0 就是 1
// 当 offset_ptr[0] 是 0 时：表示 i_node & 1 == 0 表示左节点，i_node & 1 == 1 表示右节点
// 当 offset_ptr[0] 是 1 时：表示 i_node & 1 == 1 表示左节点，i_node & 1 == 0 表示右节点
// 此处表示插入到右节点
// 直接把右节点挤到 tmp 中
if (offset_ptr[0] ^ (j & 1)){
    tmp                         = swap_node(i_node, new_node);
}
// 插入到左节点，变换为插入到右节点，然后交换逻辑顺序
else{
    offset_ptr[0]              ^= 1;                // 取反
    tmp                         = get_node(j ^ 1);  // 既然 j 表示左节点，那么 j ^ 1 表示的就是右节点
    set_node(j ^ 1, new_node);
}

// LV04                                00
// LV03                00                              00        
// LV02        00              00              00              00
// LV01    00      00      00      00      00      00      00      00
// LV00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00  00
for(bool go_on = true; go_on;){
    // 令 L 表示左节点，R 表示右节点
    // 假设 j = 11011B
    // 则经过的节点为
    lv                          = index_of_first_reset(i_node);
    i_node                     |= uxx(1) << lv;

    // 当替换到超出范围时，表示这是最后一次替换
    // 并让 j 进行饱和运算
    if (i_node >= cnt){
        i_node                  = cnt - 1;
        go_on                   = false;
    }

    iteration_by(i_node, [&](uxx arg_lv, uxx arg_i_node, uxx * arg_offset_ptr){
        if (arg_lv == lv){
            uxx mask            = (uxx(1) << lv) - 1;
            arg_offset_ptr[0]   = mask & (arg_offset_ptr[0] + 1); // 饱和运算
        }
        if (arg_lv == 0){
            auto & local_last   = get_node(arg_i_node);
            swap(xref local_last, xref tmp);
        }
    });
}

struct pair{
    uxx     i_node;
    uxx     i_offset;
};

uxx iteration_by(uxx index, callback){
    uxx     lv              = height();
    uxx     i               = index;
    uxx     j               = 0;
    uxx     half            = capacity() / 2;
    uxx     cnt             = count();
    uxx *   offset_ptr;
    uxx     offset;

    while(lv > 0){
        lv                 -= 1;
        offset_ptr          = & get_offset(lv, j);
        offset              = offset_ptr[0];
        i                   = i < offset ? cnt - offset + i : offset - i;
        j                 <<= 1;

        // 走右边
        if (i >= half){
            j              += 1;
            i              -= half;
        }
        callback(lv, j, offset_ptr);
    }
}

replace_last(uxx lv, uxx index, first){
    uxx   mask          = (uxx(1) << lv) - 1;
    uxx   j             = iteration_to(lv, index);
    uxx   cnt           = count();
    uxx & offset        = get_offset(lv, j);

    // 
    iteration_to(lv, index);

    // 饱和运算
    offset              = mask & (offset + 1);
}


*/

int main(){
    xuser::test();
    return 0;
}