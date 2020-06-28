#ifndef xpack_talk_about_multi_barrier
#define xpack_talk_about_multi_barrier
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::talk_about_multi_barrier
    #include<thread>
    #include"define/base_type.hpp"
    #include"io/tty.hpp"
    #include"lock/multi_barrier.hpp"

    namespace xuser{
        volatile uxx cnt = 0;
        inc::multi_barrier mutex;

        void test(){
            using namespace inc;
            auto add = [](voidp key){
                for(uxx i = 1; i <= 10000000; i++){
                    while(mutex.try_lock(uxx(key)) == lock_state_t::blocked){
                        std::this_thread::yield();
                    }
                    if (i % 10000 == 0){
                        tty.write_line(key, ":", i);
                    }
                    cnt = cnt + 1;
                    mutex.unlock(uxx(key));
                }
            };

            std::thread a(add, voidp(59369));
            std::thread b(add, voidp(59377));
            std::thread c(add, voidp(59387));
            std::thread d(add, voidp(59393));

            a.join();
            b.join();
            c.join();
            d.join();
            tty.write_line(cnt);
        }
    }

    int main(){
        xuser::test();
        return 0;
    }
    #pragma pop_macro("xuser")
#endif
