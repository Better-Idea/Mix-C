#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread_self::inc
#include"concurrency/thread_self.hpp"
#include<pthread.h>
#include<time.h>
#include<unistd.h>

namespace mixc::concurrency_thread_self::origin{
    void thread_self::sleep(uxx millisecond){
        while(true){
            if (millisecond > 1000){
                usleep(1000 * 1000);
                millisecond -= 1000;
            }
            else{
                usleep(millisecond * 1000);
                break;
            }
        }
    }

    void thread_self::yield(){
        pthread_yield();
    }
}
