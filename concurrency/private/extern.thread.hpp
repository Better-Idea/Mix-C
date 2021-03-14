#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure.hpp"
#include"macro/xdefer.hpp"
#include"memop/cast.hpp"
#include"memory/allocator.hpp"

#if xis_windows
#include<windows.h>
#define xapi            WINAPI
#define xhandler_t      HANDLE
#define xret_t          DWORD
#elif xis_linux
#include<bits/local_lim.h>
#include<pthread.h>
#define xapi
#define xhandler_t      pthread_t
#define xret_t          voidp
#endif

namespace mixc::concurrency_thread{
    enum{ aligned_stack_size = 64 * 1024 };

    inline auto xapi thread_entry(voidp ptr){
        auto lambda                 = inc::cast<clambda>(ptr);

        // windows 分离线程需要自己释放线程句柄，但是未用到信号量
        // 释放 lambda
        if (lambda.invoke(); lambda.is_detached()){
            #if xis_windows
                CloseHandle(lambda.handler());
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
    thread::thread(clambda && lambda) : plambda(/*默认为 nullptr*/){
        if (not lambda.is_valid()){
            return;
        }

        // 提前获取 is_detached 属性，避免 lambda 在创建的线程中释放后产生非法访问
        auto   is_detached          = lambda.is_detached();
        auto   is_fail              = true;

        xdefer{
            if (is_fail){
                lambda.release();

                // 设置当前失败状态
                plambda.im_initialize_fail();
            }
        };

        #if xis_windows
            if (not is_detached) {
                if (lambda.semaphore_for_join(CreateSemaphoreA(nullptr/*不带名称*/, 0/*初始值*/, 1/*最大值*/, nullptr));
                    lambda.semaphore_for_join() == nullptr){
                    return;
                }
            }

            lambda.handler(
                CreateThread(
                    nullptr,
                    aligned_stack_size, 
                    & thread_entry, 
                    inc::cast<voidp>(lambda), 
                    0/*立即运行*/, 
                    nullptr
                )
            );

            if (lambda.handler() == nullptr){
                if (not is_detached){ // 顺带释放信号量句柄
                    CloseHandle(lambda.semaphore_for_join());
                }
                return;
            }
        #else
            auto conf               = pthread_attr_t{};
            auto handler            = pthread_t{};
            pthread_attr_init(& conf);
            pthread_attr_setdetachstate(& conf, is_detached);
            pthread_attr_setstacksize(& conf, aligned_stack_size);
            
            // fail
            if (pthread_create(& handler, & conf, & thread_entry, mem) != 0){
                return;
            }
            else{
                lambda.handler(voidp(handler));
            }
        #endif

        // 抵达这里表示初始化成功了
        // 设置 is_fail = false，指示之前的 xdefer 不再释放 lambda
        // 如果不是分离的线程，那么就需要让当前线程也指向 lambda
        if (is_fail = false; not is_detached){
            plambda                 = lambda;
        }
    }

    thread::~thread(){
        if (auto h = inc::atom_swap<clambda>(xref the.plambda, clambda{}); 
            h.is_valid()){

            #if xis_windows
                // 必须接收返回值，不能使用 nullptr
                DWORD ret_code = 0;

                for(WaitForSingleObject(h.semaphore_for_join(), INFINITE);
                    GetExitCodeThread(h.handler(), & ret_code) == STILL_ACTIVE;){
                    inc::thread_self::yield();
                }

                CloseHandle(h.handler());
                CloseHandle(h.semaphore_for_join());
            #elif xis_linux
                pthread_join(h.handler(), nullptr);
            #endif

            h.release();
        }
    }
}

#undef  xapi
#undef  xhandler_t
#undef  xret_t
