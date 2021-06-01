#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"macro/xdefer.hpp"
#include"macro/xvolatile.hpp"
#include"memop/cast.hpp"
#include"utils/allocator.hpp"

#if xis_windows
#include<windows.h>
#define xapi            WINAPI
#define xret_t          DWORD
#elif xis_linux
#include<pthread.h>
#define xapi
#define xret_t          voidp
#endif

namespace mixc::concurrency_thread{
    enum{ aligned_stack_size = 64 * 1024 };

    #ifndef xdecl_l_lambda
        #define xdecl_l_lambda
        inline thread_local clambda l_lambda;
    #endif

    #if xis_linux
    extern void init_mutex(pthread_mutex_t * mutex){
        auto mutex_attr         = pthread_mutexattr_t{};
        pthread_mutexattr_init(& mutex_attr);
        pthread_mutexattr_setpshared(& mutex_attr, PTHREAD_PROCESS_PRIVATE);
        pthread_mutexattr_settype(& mutex_attr, PTHREAD_MUTEX_NORMAL);
        pthread_mutexattr_setprotocol(& mutex_attr, PTHREAD_PRIO_NONE);
        pthread_mutex_init(mutex, & mutex_attr);

        // 默认上一次锁，下一次调用时直接阻塞
        pthread_mutex_lock(mutex);
    }
    #endif

    inline auto xapi thread_entry(voidp ptr){
        auto lambda                 = inc::cast<clambda>(ptr);
        l_lambda                    = lambda;

        #if xis_linux
            l_lambda.handler(voidp(pthread_self()));
        #endif

        // windows 分离线程需要自己释放线程句柄
        // 释放 lambda
        if (lambda.invoke(); lambda.is_detached()){
            #if xis_windows
                CloseHandle(lambda.handler());
                CloseHandle(lambda.semaphore_for_suspend());
            #else
                pthread_mutex_destroy((pthread_mutex_t *)lambda.semaphore_for_suspend());
                inc::free(lambda.semaphore_for_suspend(), xmemory_sizeof(pthread_mutex_t));
            #endif

            lambda.release();
        }
        // windows 非分离线程需要激活等待该线程退出的父线程
        // 资源释放由父线程完成
        else{
            #if xis_windows
                ReleaseSemaphore(lambda.semaphore_for_join(), 1/*给一个资源*/, nullptr);
            #endif
        }
        return xret_t{};
    }
}

namespace mixc::concurrency_thread::origin{
    thread::thread(clambda && lambda) : m_lambda(/*默认为 nullptr*/){
        if (not lambda.is_valid()){
            return;
        }
        else{
            // 由于线程创建就运行了，所以需要提前准备好该字段
            m_lambda                 = lambda;
        }

        // 提前获取 is_detached 属性，避免 lambda 在创建的线程中释放后产生非法访问
        auto   is_detached          = lambda.is_detached();
        auto   is_fail              = true;

        xdefer{
            if (is_fail){
                lambda.release();

                // 设置当前失败状态
                m_lambda.im_initialize_fail();
            }
        };

        #if xis_windows
            if (is_detached) {
                ; // pass
            }
            else if (lambda.semaphore_for_join(CreateSemaphoreA(nullptr/*不带名称*/, 0/*初始值*/, 1/*最大值*/, nullptr));
                lambda.semaphore_for_join() == nullptr){
                return;
            }

            if (lambda.semaphore_for_suspend(CreateSemaphoreA(nullptr/*不带名称*/, 0/*初始值*/, 1/*最大值*/, nullptr));
                lambda.semaphore_for_suspend() == nullptr){
                CloseHandle(lambda.semaphore_for_join());
                return;
            }

            lambda.handler(
                CreateThread(
                    nullptr,
                    aligned_stack_size, 
                    & thread_entry, 
                    inc::cast<voidp>(lambda), 
                    CREATE_SUSPENDED/*不立即运行，等待句柄设置完全*/, 
                    nullptr
                )
            );

            if (lambda.handler() == nullptr){
                if (not is_detached){ // 顺带释放信号量句柄
                    CloseHandle(lambda.semaphore_for_join());
                }
                CloseHandle(lambda.semaphore_for_suspend());
                return;
            }
            // 由于在线程内部调用 GetCurrentThread 得到的并不是真实句柄
            // 所以等待 lambda.handler 设置完成后再唤醒子线程
            else{
                ResumeThread(lambda.handler());
            }

        #elif xis_linux
            auto conf               = pthread_attr_t{};
            auto handler            = pthread_t{};
            auto mutex_attr         = pthread_mutexattr_t{};
            auto mutex              = inc::alloc<pthread_mutex_t>();
            auto arg                = inc::cast<voidp>(lambda);

            if (mutex == nullptr){
                return;
            }

            pthread_attr_init(& conf);
            pthread_attr_setdetachstate(& conf, is_detached);
            pthread_attr_setstacksize(& conf, aligned_stack_size);
            init_mutex(mutex);

            // 先设置，这样在 thread 中就可以读的到
            lambda.semaphore_for_suspend(mutex);
            
            // fail
            if (pthread_create(& handler, & conf, & thread_entry, arg) != 0){
                pthread_mutex_destroy(mutex);
                inc::free(mutex, xmemory_sizeof(pthread_mutex_t));
                return;
            }
            else{
                lambda.handler(voidp(handler));
            }

        #else
            #error "pending"
        #endif

        // 抵达这里表示初始化成功了
        // 设置 is_fail = false，指示之前的 xdefer 不再释放 lambda
        // 如果不是分离的线程，那么就需要让当前线程也指向 lambda
        is_fail                     = false;
    }

    thread::~thread(){
        if (auto h = inc::atom_swap<clambda>(xref the.m_lambda, clambda{}); 
            h.is_valid() and not h.is_detached()){

            #if xis_windows
                // 必须接收返回值，不能使用 nullptr
                DWORD ret_code = 0;

                for(WaitForSingleObject(h.semaphore_for_join(), INFINITE);
                    GetExitCodeThread(h.handler(), & ret_code) == STILL_ACTIVE;){
                    inc::thread_self::yield();
                }

                CloseHandle(h.handler());
                CloseHandle(h.semaphore_for_join());
                CloseHandle(h.semaphore_for_suspend());
            #elif xis_linux
                pthread_join(pthread_t(h.handler()), nullptr);
                pthread_mutex_destroy((pthread_mutex_t *)h.semaphore_for_suspend());
                inc::free(h.semaphore_for_suspend(), xmemory_sizeof(pthread_mutex_t));
            #endif

            h.release();
        }
    }

    void thread::resume(){
        inc::thread_self::resume(the.id());
    }
}

#undef  xapi
#undef  xret_t
