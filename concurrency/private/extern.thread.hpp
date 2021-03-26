#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"define/base_type.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure/platform.hpp"
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
        inline thread_local voidp l_lambda;

        inline auto clambda_wrap(){
            return (clambda *)(xref l_lambda);
        }
    #endif

    inline auto xapi thread_entry(voidp ptr){
        auto lambda                 = inc::cast<clambda>(ptr);
        l_lambda                    = inc::cast<clambda_meta *>(ptr);

        // windows 分离线程需要自己释放线程句柄，但是未用到信号量
        // 释放 lambda
        if (lambda.invoke(); lambda.is_detached()){
            #if xis_windows
                CloseHandle(lambda.handler());
                CloseHandle(lambda.semaphore_for_suspend());
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
            else{
                plambda             = lambda;
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
                    0/*立即运行*/, 
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
        is_fail                     = false;
        plambda                     = lambda;
    }

    thread::~thread(){
        if (auto h = inc::atom_swap<clambda>(xref the.plambda, clambda{}); 
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
                pthread_join(h.handler(), nullptr);
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
