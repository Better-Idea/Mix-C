#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread_self::inc
#include"concurrency/thread_self.hpp"
#include"configure.hpp"

#if xis_windows
#include<windows.h>
#elif xis_linux
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#endif

namespace mixc::concurrency_thread_self::origin{
    void thread_self::sleep(uxx millisecond){
        #ifdef xis_windows
            Sleep(millisecond);
        #elif  xis_linux
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
        #else
            #error""
        #endif
    }

    void thread_self::yield(){
        #if xis_windows
            Sleep(0);
        #elif xis_linux
            pthread_yield();
        #else
            #error""
        #endif
    }
}
