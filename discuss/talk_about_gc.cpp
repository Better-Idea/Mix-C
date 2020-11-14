#ifndef xpack_talk_about_gc
#define xpack_talk_about_gc
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::talk_about_gc::inc
#include"docker/shared_array.hpp"
#include"docker/shared_ptr.hpp"
#include"macro/xhint.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::talk_about_gc::inc{
    struct ax;
    struct bx;

    xstruct(
        xname(ax),
        xpubf(a, shared_ptr<ax>),
        xpubf(b, shared_ptr<bx>),
        xpubf(c, shared_ptr<uxx>)
    ) $

    xstruct(
        xname(bx),
        xpubf(a, shared_ptr<ax>),
        xpubf(b, shared_ptr<bx>)
    ) $

    struct N1_t;
    struct N2_t;
    struct N3_t;

    xstruct(
        xname(N1_t),
        xpubf(na, shared_ptr<N2_t>),
        xpubf(nb, shared_ptr<N2_t>),
        xpubf(name, asciis)
    ) $

    xstruct(
        xname(N2_t),
        xpubf(na, shared_ptr<N2_t>),
        xpubf(nb, shared_ptr<N2_t>),
        xpubf(nc, shared_ptr<N3_t>),
        xpubf(name, asciis)
    ) $

    xstruct(
        xname(N3_t),
        xpubf(name, asciis)
    ) $

    xstruct(
        xname(N4_t),
        xpubf(n, shared_array<shared_ptr<N4_t>>)
    ) $

    void test(){
        using namespace inc;

        {
            xhint("step 1:", used_bytes());
            shared_ptr<uxx> c(ini_now);
            {
                xhint("step 2:", used_bytes());
                shared_ptr<ax> x(ini_now);
                {
                    xhint("step 3:", used_bytes());
                    shared_ptr<ax> a(ini_now);
                    xhint("step 4:", used_bytes());
                    shared_ptr<bx> b(ini_now);
                    xhint("step 5:", used_bytes());
                    x->a = a;
                    x->b = b;
                    x->c = c;
                    a->a = x;
                    a->b = b;
                    a->c = c;
                    b->a = x;
                    b->b = b;
                    xhint("step 6:", used_bytes());
                }
                xhint("step 7:", used_bytes());
            }
            xhint("step 8:", used_bytes());
        }
        xhint("step 9:", used_bytes());

        {
            shared_ptr<N1_t> n1(ini_now);{ n1->name = "n1";
                shared_ptr<N2_t> n2_1(ini_now);{ n2_1->name = "n2_1";
                    shared_ptr<N2_t> n2_2(ini_now);{ n2_2->name = "n2_2";
                        shared_ptr<N2_t> n2_3(ini_now);{ n2_3->name = "n2_3";
                            shared_ptr<N2_t> n2_4(ini_now);{ n2_3->name = "n2_4";
                                shared_ptr<N3_t> n3(ini_now); n3->name = "n3";
                                n1->na      = n2_1;
                                n2_1->na    = n2_2;
                                n2_2->na    = n2_3;
                                n2_1->nb    = n2_3;
                                n2_3->na    = n2_4;
                                n2_4->na    = n2_2;
                                n2_3->nc    = n3;
                            }
                            xhint("step:10", used_bytes());
                        }
                        xhint("step:11", used_bytes());
                    }
                    xhint("step:12", used_bytes());
                }
                xhint("step:13", used_bytes());
            }
            xhint("step:13", used_bytes());
        }
        xhint("step:14", used_bytes());

        {
            // 其实我们更推荐这么写
            // 这样可以减少栈上环对象的个数，避免无用的析构操作
            shared_ptr<N3_t> n3(ini_now);
            shared_ptr<N1_t> n1(ini_now);
            auto & n2_1 = n1->na(ini_now);
            auto & n2_2 = n2_1->na(ini_now);
            auto & n2_3 = n2_2->na(ini_now);
            auto & n2_4 = n2_3->na(ini_now);
            n2_1->nb = n2_3;
            n2_3->nc = n3;
            n2_4->na = n2_2;
            xhint("step:15", used_bytes());
        }
        xhint("step:16", used_bytes());

        {
            shared_ptr<N4_t> n4(ini_now);
            n4->n(length{4}, n4); // 给数组每个都赋值 n4
            xhint("step:17", used_bytes());
        }
        xhint("step:18", used_bytes());
    }
}

int main(){
    mixc::talk_about_gc::inc::test();
    return 0;
}

#endif
