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
        do{
            usleep(millisecond % 1000 * 1000);
            millisecond -= millisecond % 1000;
        }while(millisecond != 0);
    }

    void thread_self::yield(){
        pthread_yield();
    }
}
