#define xuser mixc::powerful_cat
#include"algo/heap_root.hpp"
#include"docker/array.hpp"
#include"math/random.hpp"
#include"mixc.hpp"

namespace xuser{
    void demo(){
        uxx demo = 0;

        // 演示：heap_root 默认生成小根堆
        {
            xhint(demo);
            demo               += 1;

            array<uxx, 6> ary;

            xhint("origin order");

            for(uxx i = 0; i < ary.length(); i++){
                uxx length      = i;
                uxx v           = random<uxx>() % 10;
                heap_root::push(ary, length, v);
                xhint(v);
            }

            xhint("asc order");

            for(uxx i = ary.length(); i-- > 0;){
                uxx length      = i + 1;
                uxx v           = heap_root::pop(ary, length);
                xhint(v);
            }
        }

        // 演示：heap_root 生成大根堆
        {
            xhint(demo);
            demo               += 1;

            array<uxx, 6> ary;

            xhint("origin order");

            for(uxx i = 0; i < ary.length(); i++){
                uxx length      = i + 1;
                uxx v           = random<uxx>() % 10;

                // 这里使用第四个参数，让它使用负逻辑比较
                heap_root::push(ary, length, v, default_compare_neg<uxx>);
                xhint(v);
            }

            xhint("des order");

            for(uxx i = ary.length(); i-- > 0;){
                uxx length      = i + 1;

                // pop 的比较回调要和 push 的保持一致
                uxx v           = heap_root::pop(ary, length, default_compare_neg<uxx>);
                xhint(v);
            }
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}