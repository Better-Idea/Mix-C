#define xuser mixc::powerful_cat
#include"io/tty.hpp"
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
    }
}

int main(){
    xuser::demo();
    return 0;
}