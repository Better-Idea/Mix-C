#ifndef xpack_talk_about_gc
#define xpack_talk_about_gc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::talk_about_gc::inc
#include"docker/shared_array.hpp"
#include"docker/shared.hpp"
#include"macro/xhint.hpp"
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

    void test(){
        // CAT WARNING：
        // 以下千层饼结构仅用于演示指针指针在作用域内外的行为，实际代码编写请避免以下风格
        counter step = 0;
        {
            xhint(step, used_bytes());
            shared<uxx> c { init_by_default };
            {
                xhint(step, used_bytes());
                shared<ax> x { init_by_default };
                {
                    xhint(step, used_bytes());
                    shared<ax> a{ init_by_default };
                    xhint(step, used_bytes());
                    shared<bx> b{ init_by_default };
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
                xhint(step, used_bytes());
            }
            xhint(step, used_bytes());
        }
        xhint(step, used_bytes());

        {
            shared<N1_t> n1{ init_by_default };{ 
                n1->name = "n1"; 
                xhint(step, used_bytes());
                shared<N2_t> n2_1{ init_by_default };{ 
                    n2_1->name = "n2_1";
                    xhint(step, used_bytes());
                    shared<N2_t> n2_2{ init_by_default };{ 
                        n2_2->name = "n2_2";
                        xhint(step, used_bytes());
                        shared<N2_t> n2_3{ init_by_default };{
                            n2_3->name = "n2_3";
                            xhint(step, used_bytes());
                            shared<N2_t> n2_4{ init_by_default };{
                                n2_3->name = "n2_4";
                                xhint(step, used_bytes());

                                shared<N3_t> n3{ init_by_default }; 
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
                            xhint(step, used_bytes());
                        }
                        xhint(step, used_bytes());
                    }
                    xhint(step, used_bytes());
                }
                xhint(step, used_bytes());
            }
            xhint(step, used_bytes());
        }
        xhint(step, used_bytes());

        {
            // 其实我们更推荐这么写
            // 这样可以减少栈上环对象的个数，避免无用的析构操作
            shared<N3_t> n3{ init_by_default };
            shared<N1_t> n1{ init_by_default };
            auto & n2_1 = n1->na    = { init_by_default };
            auto & n2_2 = n2_1      = { init_by_default };
            auto & n2_3 = n2_2      = { init_by_default };
            auto & n2_4 = n2_3      = { init_by_default };
            n2_1->nb                = n2_3;
            n2_3->nc                = n3;
            n2_4->na                = n2_2;
            xhint(step, used_bytes());
        }
        xhint(step, used_bytes());

        {
            shared<N4_t> n4         = { init_by_default };
            n4->n                   = { n4, n4, n4, n4 }; // 创建长度为 4 的数组，并将每个元素赋值为 n4
            xhint(step, used_bytes());
        }
        xhint(step, used_bytes());
    }
}

int main(){
    mixc::talk_about_gc::inc::test();
    return 0;
}

#endif
