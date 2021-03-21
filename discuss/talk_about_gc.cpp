#define xuser mixc::talk_about_gc::inc
#include"concurrency/thread_self.hpp"
#include"docker/shared_array.hpp"
#include"docker/shared.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

namespace xuser{
    struct ax;
    struct bx;

    xstruct(
        xname(ax),
        xpubf(a, shared<ax>),
        xpubf(b, shared<bx>),
        xpubf(c, shared<uxx>)
    ) $

    xstruct(
        xname(bx),
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
}

int run(){
    using namespace xuser;

    // CAT WARNING：
    // 以下千层饼结构仅用于演示指针指针在作用域内外的行为，实际代码编写请避免以下风格
    counter step = 0;
    {
        xhint(step, used_bytes());
        shared<uxx> c { default_init_by };
        {
            xhint(step, used_bytes());
            shared<ax> x { default_init_by };
            {
                xhint(step, used_bytes());
                shared<ax> a{ default_init_by };
                xhint(step, used_bytes());
                shared<bx> b{ default_init_by };
                xhint(step, used_bytes());
                x->a = a;
                x->b = b;
                x->c = c;
                a->a = x;
                a->b = b;
                a->c = c;
                b->a = x;
                b->b = b;
                xhint(step, used_bytes());
            }
            thread_self::gc_sync();
            xhint(step, used_bytes());
        }
        thread_self::gc_sync();
        xhint(step, used_bytes());
    }
    thread_self::gc_sync();
    xhint(step, used_bytes());

    {
        shared<N1_t> n1{ default_init_by };{ 
            n1->name = "n1"; 
            xhint(step, used_bytes());
            shared<N2_t> n2_1{ default_init_by };{ 
                n2_1->name = "n2_1";
                xhint(step, used_bytes());
                shared<N2_t> n2_2{ default_init_by };{ 
                    n2_2->name = "n2_2";
                    xhint(step, used_bytes());
                    shared<N2_t> n2_3{ default_init_by };{
                        n2_3->name = "n2_3";
                        xhint(step, used_bytes());
                        shared<N2_t> n2_4{ default_init_by };{
                            n2_3->name = "n2_4";
                            xhint(step, used_bytes());

                            shared<N3_t> n3{ default_init_by }; 
                            n3->name = "n3";
                            xhint(step, used_bytes());

                            n1->na      = n2_1;
                            n2_1->na    = n2_2;
                            n2_2->na    = n2_3;
                            n2_1->nb    = n2_3;
                            n2_3->na    = n2_4;
                            n2_4->na    = n2_2;
                            n2_3->nc    = n3;
                        }
                        thread_self::gc_sync();
                        xhint(step, used_bytes());
                    }
                    thread_self::gc_sync();
                    xhint(step, used_bytes());
                }
                thread_self::gc_sync();
                xhint(step, used_bytes());
            }
            thread_self::gc_sync();
            xhint(step, used_bytes());
        }
        thread_self::gc_sync();
        xhint(step, used_bytes());
    }
    thread_self::gc_sync();
    xhint(step, used_bytes());

    {
        // 其实我们更推荐这么写
        // 这样可以减少栈上环对象的个数，避免无用的析构操作
        shared<N3_t> n3{ default_init_by };
        shared<N1_t> n1{ default_init_by };
        auto & n2_1 = n1->na    = { default_init_by };
        auto & n2_2 = n2_1      = { default_init_by };
        auto & n2_3 = n2_2      = { default_init_by };
        auto & n2_4 = n2_3      = { default_init_by };
        n2_1->nb                = n2_3;
        n2_3->nc                = n3;
        n2_4->na                = n2_2;
        xhint(step, used_bytes());
    }
    thread_self::gc_sync();
    xhint(step, used_bytes());

    {
        shared<N4_t> n4         = { default_init_by };
        n4->n                   = { n4, n4, n4, n4 }; // 创建长度为 4 的数组，并将每个元素赋值为 n4
        xhint(step, used_bytes());
    }
    thread_self::gc_sync();
    xhint(step, used_bytes());
    return 0;
}
