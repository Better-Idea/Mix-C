#define xuser mixc::powerful_cat
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

namespace xuser{
    void demo(){
        using namespace ph;
        counter demo = 0;

        auto alloc = [](uxx length){
            static char buf[128];
            buf[length] = '\0';
            return buf;
        };

        // 演示：小写 16 进制占位符 'h'
        {
            xhint(demo, "placeholder 'h'");

            u32 value           = 0x12ab;
            c08 content         = h{value}.format(alloc);

            // content:"12ab"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // content:"abc1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = h{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：大写 16 进制占位符 'H'
        {
            xhint(demo, "placeholder 'H'");

            u32 value           = 0x12ab;
            c08 content         = H{value}.format(alloc);

            // content:"12AB"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // content:"ABC1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = H{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：保留前导零的小写 16 进制占位符 'zh'
        {
            xhint(demo, "placeholder 'zh'");

            u32 value           = 0x12ab;
            c08 content         = zh{value}.format(alloc);

            // 由于 u32 具有 8 位 16 进制，value 实际的值只占用 4 位，所以会有 4 个前导零
            // 前导零的个数与 value 数值类型的位数有关
            // content:"000012ab"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // 对于 32bit 系统：
            // content:"0abc1234"
            // 对于 64bit 系统：
            // content:"000000000abc1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = zh{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：保留前导零的大写 16 进制占位符 'zH'
        {
            xhint(demo, "placeholder 'zH'");

            u32 value           = 0x12ab;
            c08 content         = zH{value}.format(alloc);

            // 由于 u32 具有 8 位 16 进制，value 实际的值只占用 4 位，所以会有 4 个前导零
            // 前导零的个数与 value 数值类型的位数有关
            // content:"000012AB"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // 对于 32bit 系统（指针是 32bit）：
            // content:"0ABC1234"
            // 对于 64bit 系统（指针是 64bit）：
            // content:"000000000ABC1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = zH{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：带 "0x" 前缀的小写 16 进制占位符 'x'
        {
            xhint(demo, "placeholder 'x'");

            u32 value           = 0x12ab;
            c08 content         = x{value}.format(alloc);

            // content:"0x12ab"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // content:"0xabc1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = x{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：带 "0x" 前缀的大写 16 进制占位符 'X'
        {
            xhint(demo, "placeholder 'X'");

            u32 value           = 0x12ab;
            c08 content         = X{value}.format(alloc);

            // content:"0x12AB"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // content:"0xABC1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = X{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：带 "0x" 前缀和前导零的小写 16 进制占位符 'zx'
        {
            xhint(demo, "placeholder 'zx'");

            u32 value           = 0x12ab;
            c08 content         = zx{value}.format(alloc);

            // 由于 u32 具有 8 位 16 进制，value 实际的值只占用 4 位，所以会有 4 个前导零
            // 前导零的个数与 value 数值类型的位数有关
            // content:"0x000012ab"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // 对于 32bit 系统（指针是 32bit）：
            // content:"0x0abc1234"
            // 对于 64bit 系统（指针是 64bit）：
            // content:"0x000000000abc1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = zx{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：带 "0x" 前缀和前导零的大写 16 进制占位符 'zX'
        {
            xhint(demo, "placeholder 'zX'");

            u32 value           = 0x12ab;
            c08 content         = zX{value}.format(alloc);

            // 由于 u32 具有 8 位 16 进制，value 实际的值只占用 4 位，所以会有 4 个前导零
            // 前导零的个数与 value 数值类型的位数有关
            // content:"0x000012AB"
            xhint(value, content);

            // 指针类型也会被转换成数值
            // 对于 32bit 系统（指针是 32bit）：
            // content:"0x0ABC1234"
            // 对于 64bit 系统（指针是 64bit）：
            // content:"0x000000000ABC1234"
            voidp ptr           = (voidp)0xabc1234;
            content             = zX{ptr}.format(alloc);

            xhint(ptr, content);
        }

        // 演示：8 进制占位符 'o'
        {
            xhint(demo, "placeholder 'o'");

            u32 value           = 01234; // 以 0 开头表示该数值是 8 进制
            c08 content         = o{value}.format(alloc);

            xhint(value, content);
        }

        // 演示：带前导零的 8 进制占位符 'zo'
        {
            xhint(demo, "placeholder 'zo'");

            u32 value           = 01234; // 以 0 开头表示该数值是 8 进制
            c08 content         = zo{value}.format(alloc);

            // value 有 32bit，实际上可以分成 1 个 2bit + 10 个 3bit 的 8 进制数
            // value = 01234(oct)，只占用 4 位八进制数，所以有 7 个前导零
            xhint(value, content);
        }

        // 演示：2 进制占位符 'b'
        {
            xhint(demo, "placeholder 'b'");

            u32 value           = 0b10010;  // 以 0b 开头的常量为 2 进制数字
            c08 content         = b{value}.format(alloc);

            xhint(value, content);
        }

        // 演示：带前导零的 8 进制占位符 'zo'
        {
            xhint(demo, "placeholder 'zb'");

            // 以 0b 开头的常量为 2 进制数字，这里避免前导零太多所以选择 u08 示意
            u08 value           = 0b10010;
            c08 content         = zb{value}.format(alloc);

            // value 有 8bit
            // 实际显式的是 '00010010'，会有 3 个前导零
            xhint(value, content);
        }

        // 演示：通用占位符 'v'
        {
            xhint(demo, "placeholder 'v'");

            c08 content         = v{996}.format(alloc);
            xhint(content);

            // \v 为占位符标识
            // 那么后续元素的内容都会填充到 \v 对应的位置
            content             = v{2021,1,1}.format("\v-\v-\v", alloc);

            // 显示 "2021-1-1"
            xhint(content);

            // 'v' 还支持占位符的嵌套
            content             = v{ h(123), o(123), b(123) }.format("0x\v=0\v=0b\v", alloc);
            xhint(content);
        }

        // 演示：左对齐
        {
            // ph 命名空间下的所有占位符都支持对齐
            xhint(demo, "align left 'placeholder::l()'");

            // 左对齐 10 个字符单位
            c08 content         = v{ v{996}.l(10), "is horrible" }.format(alloc);

            // 显示 "996       is horrible"
            xhint(content);

            // 左对齐 10 个字符单位，空出的部分用 '?' 填充
            content             = v{ v{996}.l(10, '?'), "is horrible" }.format(alloc);

            // 显示 "996???????is horrible"
            xhint(content);
        }

        // 演示：右对齐
        {
            // ph 命名空间下的所有占位符都支持对齐
            xhint(demo, "align right 'placeholder::r()'");

            // 右对齐 10 个字符单位，空出的部分用 ' ' 填充
            c08 content         = v{ "the shit working mode", v{996}.r(10)  }.format(alloc);

            // 显示 "the shit working mode       996"
            xhint(content);

            // 右对齐 10 个字符单位，空出的部分用 '?' 填充
            content             = v{ "the shit working mode", v{996}.r(10, '-')  }.format(alloc);

            // 显示 "the shit working mode-------996"
            xhint(content);
        }

        // 演示：居中
        {
            // ph 命名空间下的所有占位符都支持对齐
            xhint(demo, "align center 'placeholder::c()'");

            // 居中 16 个字符单位，空出的部分用 ' ' 填充
            c08 content         = v{996}.c(16).format(alloc);

            // 显示 "      996       "
            xhint(content);

            // 右对齐 16 个字符单位，空出的部分用 '?' 填充
            content             = v{996}.c(16, '?').format(alloc);

            // 显示 "??????996???????"
            xhint(content);
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}