#ifndef xpack_talk_about_gc
#define xpack_talk_about_gc
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::talk_about_gc
    #include"define/base_type.hpp"
    #include"docker/array.hpp"
    #include"docker/darray.hpp"
    #include"docker/shared_ptr.hpp"
    #include"io/tty.hpp"
    #include"memory/allocator.hpp"

    namespace xuser{
        struct ax;
        struct bx;
        struct ax{
            xgc_fields(
                xiam(ax),
                xpub(a, inc::shared_ptr<ax>),
                xpub(b, inc::shared_ptr<bx>),
                xpub(c, inc::shared_ptr<uxx>)
            );
        };

        struct bx{
            xgc_fields(
                xiam(bx),
                xpub(a, inc::shared_ptr<ax>),
                xpub(b, inc::shared_ptr<bx>)
            );
        };

        struct dx;
        struct cx{
            xgc_fields(
                xiam(cx),
                xpub(items, 
                    inc::darray<
                        inc::shared_ptr<dx>, 
                        1, 
                        inc::shared_ptr<cx>
                    >
                )
            );
        };

        struct dx{
            xgc_fields(
                xiam(dx),
                xpub(c, inc::shared_ptr<cx>)
            );
        };

        void test(){
            using namespace inc;

            {
                tty.write_line("step 1:", inc::used_bytes());
                shared_ptr<uxx> c(ini_now);
                {
                    tty.write_line("step 2:", inc::used_bytes());
                    shared_ptr<ax> x(ini_now);
                    {
                        tty.write_line("step 3:", inc::used_bytes());
                        shared_ptr<ax> a(ini_now);
                        tty.write_line("step 4:", inc::used_bytes());
                        shared_ptr<bx> b(ini_now);
                        tty.write_line("step 5:", inc::used_bytes());
                        x->a = a;
                        x->b = b;
                        x->c = c;
                        a->a = x;
                        a->b = b;
                        a->c = c;
                        b->a = x;
                        b->b = b;
                        tty.write_line("step 6:", inc::used_bytes());
                    }
                    tty.write_line("step 7:", inc::used_bytes());
                }
                tty.write_line("step 8:", inc::used_bytes());
            }
            tty.write_line("step 9:", inc::used_bytes());
        }
    }

    int main(){
        mixc::talk_about_gc::test();
        return 0;
    }
    #pragma pop_macro("xuser")
#endif
