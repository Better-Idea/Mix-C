#define xuser mixc::powerful_cat
#include"lang/cxx/align_center.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"

int run(){
    using namespace xuser;

    auto alloc = [](uxx length){
        static char buf[100];
        buf[length] = '\0';
        return buf;
    };

    c08 origin_str  = "hello";

    // 刻度
    c08 s           = "0123456789abcdef";

    // 宽度 16 个字符，让字符串居中并在两端填充空格
    c08 v           = origin_str.align_center(16, alloc);

    xhint(s);
    xhint(v);

    // 如果指定的宽度小于串长度，则会产生截断
    v               = origin_str.align_center(origin_str.length() - 1, alloc);

    xhint(s);
    xhint(v);

    // 设置两边填充的字符
    v               = origin_str.align_center(16, '-', alloc);

    xhint(s);
    xhint(v);

    // 设置两边填充的字符
    v               = origin_str.align_center(16, '-'/*左边*/, '+'/*右边*/, alloc);

    xhint(s);
    xhint(v);

    // 来点欢乐的
    origin_str      = "我"; // 不知所措
    v               = origin_str.align_center(16, '?', alloc);

    xhint(s);
    xhint(v);
    return 0;
}