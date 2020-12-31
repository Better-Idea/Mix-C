#define xuser mixc::powerful_cat
// #include"docker/shared.hpp"
#include"define/base_type.hpp"

using item_t = uxx;

struct node;

uxx     null_node       = 0;
node *  null_node_ptr   = (node *) & null_node;

typedef struct ordered_node{
    ordered_node *  prev;
    ordered_node *  next;
    item_t          value;
} onode;

struct tree{
    onode * get_onode(uxx i){

    }

    onode * append_onode(){

    }

    uxx capacity(){

    }

    uxx count(){

    }

    uxx height(){

    }

    uxx & get_offset(uxx lv, uxx i){

    }

    void insert(uxx index){

        // 先创建新节点，让元素总数 +1
        onode * new_item    = append_onode();

        uxx i               = index;
        uxx j               = 0;
        uxx cnt             = count();
        uxx lv              = height();
        uxx half            = capacity() / 2;
        uxx offset;

        if (cnt == 1){
            new_item->prev = new_item;
            new_item->next = new_item;
            return;
        }

        while(lv-- > 1){
            offset          = get_offset(lv, j);
            i               = i < offset ? cnt - offset + i : offset - i;

            // 插入左边
            if (i < half){
                j         <<= 1;
            }
            // 插入右边
            else{
                j         <<= 1;
                j          += 1;
                i          -= half;
            }
        }

        if (){

        }
    }
};

int main(){
    return 0;
}