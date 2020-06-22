#ifndef xpack_talk_gc
#define xpack_talk_gc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::talk_gc
        #include"define/base_type.hpp"
        #include"docker/shared_ptr.hpp"
        #include"io/tty.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::talk_gc{
        struct ax;
        struct bx;
        struct ax{
            xgc_fields(
                xiam(ax),
                xpub(a, inc::shared_ptr<ax>),
                xpub(b, inc::shared_ptr<bx>)
            );
        };

        struct bx{
            xgc_fields(
                xiam(bx),
                xpub(a, inc::shared_ptr<ax>),
                xpub(b, inc::shared_ptr<bx>)
            );
        };

        void test(){
            using namespace inc;
            {
                tty.write_line("step 1:", inc::used_bytes());
                shared_ptr<ax> x(ini_now);
                {
                    tty.write_line("step 2:", inc::used_bytes());
                    shared_ptr<ax> a(ini_now);
                    tty.write_line("step 3:", inc::used_bytes());
                    shared_ptr<bx> b(ini_now);
                    tty.write_line("step 4:", inc::used_bytes());
                    x->a = a;
                    x->b = b;
                    a->a = a;
                    a->b = b;
                    b->a = a;
                    b->b = b;
                    tty.write_line("step 5:", inc::used_bytes());
                }
                tty.write_line("step 6:", inc::used_bytes());
            }
            tty.write_line("step 7:", inc::used_bytes());
        }
    }

    int main(){
        mixc::talk_gc::test();
        return 0;
    }
#endif
