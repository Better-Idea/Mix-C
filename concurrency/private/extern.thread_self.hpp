#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread_self::inc
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure.hpp"
#include"gc/private/background.hpp"

#if xis_windows
#include<windows.h>
#elif xis_linux
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#endif

namespace mixc::concurrency_thread{
    #ifndef xdecl_l_lambda
        #define xdecl_l_lambda
        inline thread_local voidp l_lambda;

        inline auto clambda_wrap(){
            return (clambda *)(xref l_lambda);
        }
    #endif
}

namespace mixc::concurrency_thread_self::origin{
    #if xis_windows
        // 用 mutex 无法阻塞主线程
        inline voidp h_sem_for_suspend = CreateSemaphoreA(nullptr, 0, 1, nullptr);
    #endif

    uxx thread_self::id(){
        return uxx(mixc::concurrency_thread::l_lambda);
    }

    void thread_self::resume(uxx thread_id){
        using namespace mixc::concurrency_thread;
        auto the_thread     = inc::cast<clambda>(thread_id);
        auto is_main        = thread_id == 0;

        #ifdef xis_windows
            ReleaseSemaphore(is_main ? h_sem_for_suspend : the_thread.semaphore_for_suspend(), 1, nullptr);
        #else
            #error"pending"
        #endif
    }

    void thread_self::suspend(uxx timeout){
        using namespace mixc::concurrency_thread;
        auto is_main    = is_main_thread();
        auto wrap       = clambda_wrap();

        #ifdef xis_windows
            auto r = WaitForSingleObject(
                is_main ? h_sem_for_suspend : wrap->semaphore_for_suspend(), 
                DWORD(timeout)
            );
        #else
            #error"pending"
        #endif
    }

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
            SwitchToThread();
        #elif xis_linux
            pthread_yield();
        #else
            #error""
        #endif
    }

    void thread_self::gc_sync(){
        inc::gc_sync();
    }
}
