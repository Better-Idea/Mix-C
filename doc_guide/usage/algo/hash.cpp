#define xuser mixc::powerful_cat
#include"algo/hash.hpp"
#include"math/random.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

namespace xuser{
    void demo(){
        counter demo = 0;

        // 演示：hash 普通数值类型
        {
            xhint(demo);

            uxx v       = hash(1024);
            xhint(v);

            v           = hash('a');
            xhint(v);

            // 浮点也可以进行 hash，但需要使用者保证不存在精度问题
            v           = hash(3.1415926);
            xhint(v);
        }

        // 演示：hash 原始字符串类型
        {
            xhint(demo);

            asciis s    = "hello world";
            uxx    a    = hash("hello world");  // 实际上这两个传入的参数类型是不一样的
            uxx    b    = hash(s);              // 前者是字符数组，后者是字符指针，但我们保证了一致性
            xhint(a);
            xhint(b);
        }

        // 演示：hash 未知类型
        {
            xhint(demo);

            struct { u08 bytes[10]; }unknown;

            xhint(hash(unknown));
        }

        // 演示：使用 hash 函数第二个参数避免 hash 攻击
        {
            xhint(demo);

            // 弱口令
            xhint(hash("123456"));

            uxx r       = random<uxx>();

            // 带随机数的弱口令
            xhint(hash("123456", r));
        }

        // 演示：即使值相同，不同字节数的整数也会有不同的 hash 值
        {
            xhint(demo);

            u08 a       = 'a';
            u16 b       = 'a';

            xhint(hash(a));
            xhint(hash(b));
        }
    }
}

int main(){
    xuser::demo();
    return 0;
}