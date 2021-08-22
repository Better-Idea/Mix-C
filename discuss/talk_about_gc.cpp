#define xuser mixc::talk_about_gc::inc
#include"concurrency/thread_self.hpp"
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/thread.hpp"
#include"docker/shared_array.hpp"
#include"docker/shared.hpp"
#include"utils/memory.hpp"
#include"utils/counter.hpp"
#include"mixc.hpp"
#include"utils/tick.hpp"

namespace xuser{
    struct ax;
    struct bx;

    xstruct(
        xname(ax),
        xpubf(name, asciis),
        xpubf(a, shared<ax>),
        xpubf(b, shared<bx>),
        xpubf(c, shared<uxx>)
    ) $

    xstruct(
        xname(bx),
        xpubf(name, asciis),
        xpubf(a, shared<ax>),
        xpubf(b, shared<bx>)
    ) $

    struct N1_t;
    struct N2_t;
    struct N3_t;

    xstruct(
        xname(N1_t),
        xpubf(na, shared<N2_t>),
        xpubf(nb, shared<N2_t>),
        xpubf(name, asciis)
    ) $

    xstruct(
        xname(N2_t),
        xpubf(na, shared<N2_t>),
        xpubf(nb, shared<N2_t>),
        xpubf(nc, shared<N3_t>),
        xpubf(name, asciis)
    ) $

    xstruct(
        xname(N3_t),
        xpubf(name, asciis)
    ) $

    xstruct(
        xname(N4_t),
        xpubf(n, shared_array<shared<N4_t>>)
    ) $

    xstruct(
        xname(groupa),
        xpubf(a, shared<groupa>)
    ) $

    xstruct(
        xname(groupb),
        xpubf(a, shared<groupa>),
        xpubf(b, shared<groupb>)
    ) $
}

inline void wait(){
    using namespace xuser;
    //thread_self::gc_sync();     // 等待垃圾收集完成
    //thread_self::sleep(256);    // 等待内存释放
}

#define xtest_memory_lack   1

#if xtest_memory_lack
    #define debug(...)
#else
    #define debug(...)  xhint(__VA_ARGS__)
    //#define debug(...)
#endif


void closure(){
    using namespace xuser;

    // CAT WARNING：
    // 以下千层饼结构仅用于演示指针指针在作用域内外的行为，实际代码编写请避免以下风格
    counter step = 0;
    {
        debug(step, memory::used_bytes());
        shared<uxx> c { default_init_by };
        {
            debug(step, memory::used_bytes());
            shared<ax> x { default_init_by };
            {
                x->name = "x";
                debug(step, memory::used_bytes());
                shared<ax> a{ default_init_by };
                debug(step, memory::used_bytes());
                shared<bx> b{ default_init_by };
                a->name = "a";
                b->name = "b";
                debug(step, memory::used_bytes());
                x->a = a;
                x->b = b;
                x->c = c;
                a->a = x;
                a->b = b;
                a->c = c;
                b->a = x;
                b->b = b;
                debug(step, memory::used_bytes());
            }
            wait(); debug(step, memory::used_bytes());
        }
        wait(); debug(step, memory::used_bytes());
    }
    wait(); debug(step, memory::used_bytes());

    {
        shared<N1_t> n1{ default_init_by };{ 
            n1->name = "n1"; 
            debug(step, memory::used_bytes());
            shared<N2_t> n2_1{ default_init_by };{ 
                n2_1->name = "n2_1";
                debug(step, memory::used_bytes());
                shared<N2_t> n2_2{ default_init_by };{ 
                    n2_2->name = "n2_2";
                    debug(step, memory::used_bytes());
                    shared<N2_t> n2_3{ default_init_by };{
                        n2_3->name = "n2_3";
                        debug(step, memory::used_bytes());
                        shared<N2_t> n2_4{ default_init_by };{
                            n2_3->name = "n2_4";
                            debug(step, memory::used_bytes());

                            shared<N3_t> n3{ default_init_by }; 
                            n3->name = "n3";
                            debug(step, memory::used_bytes());

                            n1->na      = n2_1;
                            n2_1->na    = n2_2;
                            n2_2->na    = n2_3;
                            n2_1->nb    = n2_3;
                            n2_3->na    = n2_4;
                            n2_4->na    = n2_2;
                            n2_3->nc    = n3;
                        }
                        wait(); debug(step, memory::used_bytes());
                    }
                    wait(); debug(step, memory::used_bytes());
                }
                wait(); debug(step, memory::used_bytes());
            }
            wait(); debug(step, memory::used_bytes());
        }
        wait(); debug(step, memory::used_bytes());
    }
    wait(); debug(step, memory::used_bytes());

    {
        // 其实我们更推荐这么写
        // 这样可以减少栈上环对象的个数，避免无用的析构操作
        shared<N3_t> n3{ default_init_by };
        debug(step, memory::used_bytes());
        shared<N1_t> n1{ default_init_by };
        debug(step, memory::used_bytes());
        auto & n2_1 = n1->na    = { default_init_by };
        auto & n2_2 = n2_1      = { default_init_by };
        auto & n2_3 = n2_2      = { default_init_by };
        auto & n2_4 = n2_3      = { default_init_by };
        n2_1->nb                = n2_3;
        n2_3->nc                = n3;
        n2_4->na                = n2_2;
    }
    wait(); debug(step, memory::used_bytes());

    {
        shared<N4_t> n4         = { default_init_by };
        n4->n                   = { n4, n4, n4, n4 }; // 创建长度为 4 的数组，并将每个元素赋值为 n4
        debug(step, memory::used_bytes());
    }
    wait(); debug(step, memory::used_bytes());

    // 一个[潜质类型]包含另一个[潜质类型]
    {
        shared<groupb> b        = default_init_by;
        {
            auto & b_a          = b->a = default_init_by;
            auto & b_b          = b->b = default_init_by;
            b_b->b              = b;
            b_a->a              = b_a;
        }
        wait(); debug(step, memory::used_bytes());
    }
    wait(); debug(step, memory::used_bytes());
}

uxx times = 0;

xinit(xuser::the_main){
    using namespace xuser;

    #if xtest_memory_lack
    xhint("go", memory::alive_object(), memory::used_bytes());
    auto max_flow = 6;
    auto max_per_term = 128'000'000;

    auto invoke = [&](){
        for(uxx i = 0; i < max_per_term; i++){
            closure();
            atom_fetch_add(xref(times), 1);
        }
    };


    for(uxx i = 0; i < max_flow; i++){
        thread go(xdetached{
            invoke();
        });
    }
    
    #else

    xhint(memory::alive_object(), memory::used_bytes(), atom_load(xref(times)));

    // closure();
    for (uxx i = 0; i < 1; i++) {
        closure();
        atom_add(xref(times), 1);
    }

    #endif

    uxx last = 0;
    uxx step = 0;

    while(max_flow * max_per_term != times) {
        step = atom_load(xref(times)) - last;
        last = atom_load(xref(times));
        xhint(step, memory::alive_pages(), atom_load(xref(times)));
        thread_self::sleep(1000);
    }
};
