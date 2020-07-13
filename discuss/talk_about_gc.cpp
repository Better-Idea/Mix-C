#ifndef xpack_talk_about_gc
#define xpack_talk_about_gc
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::talk_about_gc
    #include"define/base_type.hpp"
    #include"docker/array.hpp"
    #include"docker/darray.hpp"
    #include"docker/shared_ptr.hpp"
    #include"macro/xhint.hpp"
    #include"memory/allocator.hpp"

    namespace xuser::inc{
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
                                xhint("step:10");
                            }
                            xhint("step:11");
                        }
                        xhint("step:12");
                    }
                    xhint("step:13");
                }
                xhint("step:13");
            }
            xhint("step:14");

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
                xhint("step:15");
            }
            xhint("step:16");
        }

    }

    int main(){
        mixc::talk_about_gc::inc::test();
        return 0;
    }
    #pragma pop_macro("xuser")
#endif
