#undef  xuser
#define xuser mixc::talk_about_multi_barrier::inc
#include"concurrency/lock/multi_barrier.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"mixc.hpp"

volatile uxx            counter = 0;
xuser::multi_barrier    mutex;

int run(){
    using namespace xuser;

    auto add = [](voidp key){
        for(uxx i = 1; i <= 10000000; i++){
            while(mutex.try_lock(uxx(key)) == lock_state_t::blocked){
                thread_self::yield();
            }
            if (i % 10000 == 0){
                // tty.write_line(key, ":", i);
            }
            counter = counter + 1;
            mutex.unlock(uxx(key));
        }
    };
    {
        thread a(xjoinable{ add(voidp(59369)); });
        thread b(xjoinable{ add(voidp(59377)); });
        thread c(xjoinable{ add(voidp(59387)); });
        thread d(xjoinable{ add(voidp(59393)); });
    }

    xhint(counter);
    return 0;
}
