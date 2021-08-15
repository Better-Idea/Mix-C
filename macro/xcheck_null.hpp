#ifndef xpack_macro_xcheck_null
#define xpack_macro_xcheck_null
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xcheck_null::inc
#include"macro/xlink.hpp"
#include"define/nullref.hpp"
#pragma pop_macro("xuser")

/*
// 简要用法：
// 用于检查成员参数，以及成员参数的成员参数...，是否为 nullptr
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
(([&]() -> decltype(auto) { \
    using __target_t = decltype(a00 a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 a15 ); \
    if (auto const & __00 =      a00; __00 == nullptr) return __target_t(nullptr); else \
    if (auto const & __01 = __00 a01; __01 == nullptr) return __target_t(nullptr); else \
    if (auto const & __02 = __01 a02; __02 == nullptr) return __target_t(nullptr); else \
    if (auto const & __03 = __02 a03; __03 == nullptr) return __target_t(nullptr); else \
    if (auto const & __04 = __03 a04; __04 == nullptr) return __target_t(nullptr); else \
    if (auto const & __05 = __04 a05; __05 == nullptr) return __target_t(nullptr); else \
    if (auto const & __06 = __05 a06; __06 == nullptr) return __target_t(nullptr); else \
    if (auto const & __07 = __06 a07; __07 == nullptr) return __target_t(nullptr); else \
    if (auto const & __08 = __07 a08; __08 == nullptr) return __target_t(nullptr); else \
    if (auto const & __09 = __08 a09; __09 == nullptr) return __target_t(nullptr); else \
    if (auto const & __10 = __09 a10; __10 == nullptr) return __target_t(nullptr); else \
    if (auto const & __11 = __10 a11; __11 == nullptr) return __target_t(nullptr); else \
    if (auto const & __12 = __11 a12; __12 == nullptr) return __target_t(nullptr); else \
    if (auto const & __13 = __12 a13; __13 == nullptr) return __target_t(nullptr); else \
    if (auto const & __14 = __13 a14; __14 == nullptr) return __target_t(nullptr); else \
    if (auto const & __15 = __14 a15; __15 == nullptr) return __target_t(nullptr); else \
        return (__target_t)__15; \
})())

#define xcheck_null(...)        xlink(__xcheck_null_, core__(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))

/*

#define xuser mixc::powerful_cat
#include"io/tty.hpp"

// 代码生成
// 只要修改这个参数就可以改变链式级数
uxx max_level = 16;

xinit(xuser::the_main){
   auto gen_line = [](uxx level, asciis pad = " ") {
       for(uxx i = 0; i < level; i++) {
           xuser::tty.write(
               "a\v\v", xuser::ph::v{ i }.r(2, '0'), pad
           );
       }
   };

   xuser::tty.write("#define __xcheck_null_core__(");
   gen_line(max_level, ",");
   xuser::tty.write_line("...) \\");
   xuser::tty.write_line("(([&]() -> decltype(auto) { \\");
   xuser::tty.write("    using __target_t = decltype(");
   gen_line(max_level);
   xuser::tty.write_line("); \\");
   xuser::tty.write_line("    if (auto const & __00 =      a00; __00 == nullptr) return __target_t(nullptr); else \\");

   for(uxx i = 1; i < max_level; i++){
       xuser::tty.write_line("    if (auto const & __\v = __\v a\v; __\v == nullptr) return __target_t(nullptr); else \\", 
           xuser::ph::v{i + 0}.r(2, '0'), 
           xuser::ph::v{i - 1}.r(2, '0'), 
           xuser::ph::v{i + 0}.r(2, '0'),
           xuser::ph::v{i + 0}.r(2, '0')
       );
   }

   xuser::tty.write_line("        return (__target_t)__\v; \\", xuser::ph::v{ max_level - 1 }.r(2, '0'));
   xuser::tty.write_line("})())");
};
*/

#endif
