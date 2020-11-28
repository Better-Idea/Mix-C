#define xuser mixc::powerful_cat
#include"lang/cxx/align_right.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"

namespace xuser{
    void demo(){
        auto alloc = [](uxx length){
            static char buf[100];
            buf[length] = '\0';
            return buf;
        };

        c08 origin_str  = "hello";

        // 刻度
        c08 s           = "0123456789abcdef";

        // 宽度 16 个字符，让字符串右对齐并在左端填充空格
        c08 v           = origin_str.align_right(16, alloc);

        xhint(s);
        xhint(v);

        // 如果指定的宽度小于串长度，则会产生截断
        v               = origin_str.align_right(origin_str.length() - 1, alloc);

        xhint(s);
        xhint(v);

        // 设置左边填充的字符
        v               = origin_str.align_right(16, '-', alloc);

        xhint(s);
        xhint(v);
    }
}

int main(){
    xuser::demo();
    return 0;
}