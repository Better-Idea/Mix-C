#define xuser mixc::powerful_cat
#include"algo/distinct.hpp"
#include"docker/array.hpp"
#include"mixc.hpp"

namespace xuser{
    void demo(){
        uxx demo = 0;

        // 演示：去除重复元素，每种元素只保留一个
        {
            xhint(demo);
            demo           += 1;

            auto && buf     = array<i32, 32>{};
            auto && ary     = array_view{ 1, 1, 2, 3, 3, 2, 1 };

            // new_ary 来自回调函数返回的内容
            auto && new_ary = distinct(ary, [&](uxx length){
                // unified_seq<decltype(ary)> 会选择合适的序列接口类型
                return unified_seq<decltype(ary)>{buf, length};
            });

            // 依次打印 1, 2, 3
            new_ary.foreach([](i32 const & v){
                xhint(v);
            });
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}