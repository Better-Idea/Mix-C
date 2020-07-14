#ifndef xpack_talk_about_allocator
#define xpack_talk_about_allocator
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::talk_about_allocator
    #include"macro/xhint.hpp"
    #include"docker/array.hpp"
    #include"math/random.hpp"
    #include"memory/allocator.hpp"

    namespace xuser::inc{
        void test(){
            struct pair{ voidp ptr; uxx size; };
            static array<pair, 1024 * 1024> record;
            uxx len = 0;
            xhint(alive_pages());

            for(uxx i = 0; i < record.length(); i++){
                auto r0 = random<u08>() % 3;
                auto r1 = random<u16>() % 4096 + 1;
                auto pr = pair();

                if (r0 == 0 and len != 0){
                    pr        = record[0];
                    record[0] = record[--len];
                    free(pr.ptr, memory_size{pr.size});
                }
                else{
                    pr.size   = r1;
                    pr.ptr    = alloc<u08>(memory_size{pr.size});
                    record[len++] = pr;
                }
            }

            for(uxx i = 0; i < len; i++){
                auto pr = record[i];
                free(pr.ptr, memory_size{pr.size});
            }
            xhint(alive_pages());
        }
    }

    int main(){
        mixc::talk_about_allocator::inc::test();
        return 0;
    }
    #pragma pop_macro("xuser")
#endif
