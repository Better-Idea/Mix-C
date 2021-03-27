#undef  xuser
#define xuser mixc::concurrency_thread_self::inc
#include"configure/platform.hpp"
#include"configure/init_order.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure/switch.hpp"
#include"gc/private/background.hpp"
#include"utils/init_list.hpp"

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
        inline thread_local clambda l_lambda;
    #endif

    #if xis_linux
    extern void init_mutex(pthread_mutex_t * mutex);
    #endif
}

namespace mixc::concurrency_thread_self::origin{
    #if xis_windows
        // windows 用 mutex 无法阻塞主线程
        inline voidp h_for_suspend;
    #else
        inline pthread_mutex_t h_for_suspend;
    #endif

    xinit(inc::the_concurrency_thread_self){
        #if xis_windows
            // 用 mutex 无法阻塞主线程
            h_for_suspend   = CreateSemaphoreA(nullptr, 0, 1, nullptr);
        #elif xis_linux
            mixc::concurrency_thread::init_mutex(& h_for_suspend);
        #else
            #error "pending"
        #endif
    };

    uxx thread_self::id(){
        return inc::cast<uxx>(mixc::concurrency_thread::l_lambda);
    }

    void thread_self::resume(uxx thread_id){
        using namespace mixc::concurrency_thread;
        auto the_thread     = inc::cast<clambda>(thread_id);
        auto is_main        = thread_id == 0;

        #if xis_windows
            ReleaseSemaphore(is_main ? h_for_suspend : the_thread.semaphore_for_suspend(), 1, nullptr);
        #elif xis_linux
            pthread_mutex_unlock(
                is_main ? xref h_for_suspend : (pthread_mutex_t *)the_thread.semaphore_for_suspend()
            );
        #else
            #error "pending"
        #endif
    }

    void thread_self::suspend(uxx timeout){
        using namespace mixc::concurrency_thread;
        auto is_main    = is_main_thread();

        #if xis_windows
            auto r = WaitForSingleObject(
                is_main ? h_for_suspend : l_lambda.semaphore_for_suspend(), 
                DWORD(timeout)
            );
        #elif xis_linux
            auto mutex = is_main ? 
                xref h_for_suspend : (pthread_mutex_t *)l_lambda.semaphore_for_suspend();

            if (timeout == not_exist){
                pthread_mutex_lock(mutex);
                return;
            }

            timespec time{};
            clock_gettime(CLOCK_REALTIME, & time);
            time.tv_nsec += timeout % 1000 * 1'000'000;
            time.tv_sec  += timeout / 1000 + time.tv_nsec / 1'000'000'000;
            time.tv_nsec %= 1'000'000'000;
            pthread_mutex_timedlock(mutex, xref time);
        #endif
    }

    void thread_self::sleep(uxx millisecond){
        #if xis_windows
            Sleep(DWORD(millisecond));
        #elif xis_linux
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
            #error "pending"
        #endif
    }

    void thread_self::yield(){
        #if xis_windows
            SwitchToThread();
        #elif xis_linux
            pthread_yield();
        #else
            #error "pending"
        #endif
    }

    void thread_self::gc_sync(){
        inc::gc_sync();
    }
}
