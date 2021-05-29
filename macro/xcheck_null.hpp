#pragma once
#include"macro/xlink.hpp"

/*
// 简要用法：
// 用于成员参数，以及成员参数的成员参数...，是否为 nullptr
// 如果不为 nullptr 则返回当前字段的指针
#include"macro/xcheck_null.hpp"

struct ax;
struct bx{
    ax * the_ax;
};

struct ax{
    ax * the_ax;
    bx * the_bx;
};

void foo(ax * arg){
    if (ax * param = xcheck_null(arg->the_ax, ->the_bx, ->the_ax); param != nullptr){
        // do sth...
    }
}
*/

#define __xcheck_null_core__(a00,a01,a02,a03,a04,a05,a06,a07,a08,a09,a10,a11,a12,a13,a14,a15,...) \
a00 == nullptr or \
a00 a01 == nullptr or \
a00 a01 a02 == nullptr or \
a00 a01 a02 a03 == nullptr or \
a00 a01 a02 a03 a04 == nullptr or \
a00 a01 a02 a03 a04 a05 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 == nullptr or \
a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 a15 == nullptr ? \
    (decltype(a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 a15 )(nullptr)) : \
    (a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 a15 )

#define xcheck_null(...)        xlink(__xcheck_null_, core__(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))

/*
// 代码生成
// 只要修改这个参数就可以改变链式级数
uxx max_level = 16;

#define xuser mixc::powerful_cat
#include"macro/xlink.hpp"
#include"io/tty.hpp"

int run(){
    auto gen_line = [](uxx level, asciis pad = " "){
        for(uxx i = 0; i < level; i++){
            xuser::tty.write(
                "a\v\v",  xuser::ph::v{i}.r(2, '0'), pad
            );
        }
    };

    xuser::tty.write("#define __xcheck_null_core__(");
    gen_line(max_level, ",");
    xuser::tty.write_line("...) \\");

    for(uxx i = 0; i < max_level; i++){
        if (gen_line(i + 1); i + 1 != max_level){
            xuser::tty.write_line("== nullptr or \\");
        }
        else{
            xuser::tty.write_line("== nullptr ? \\");
        }
    }

    xuser::tty.write("    (decltype(");
    gen_line(max_level);
    xuser::tty.write_line(")(nullptr)) : \\");

    xuser::tty.write("    (");
    gen_line(max_level);
    xuser::tty.write(")");
    return 0;
}

*/
