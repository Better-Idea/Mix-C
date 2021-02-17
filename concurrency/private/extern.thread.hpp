#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure.hpp"
#include"macro/xalign.hpp"
#include"memory/private/tiny_allocator.hpp"

#if xis_windows
#include<windows.h>
#define handler_t       HANDLE
#elif xis_linux
#include<pthread.h>
#define handler_t       pthread_t
#endif

namespace mixc::concurrency_thread{
    typedef struct thread_local_layout{
        clambda                 lambda;
        handler_t               handler;
        handler_t               sem_for_join;

        xalign(64)
        u08                     lambda_buf[1];
    } * tllp;

    inline auto thread_entry(voidp ptr){
        // 使用原子操作保证其他 cpu 线程可见
        auto mem            = static_cast<tllp>(ptr);
        auto handler        = mem->handler = 
        
        #if xis_windows
            GetCurrentThread();
        #elif xis_linux
            pthread_self();
        #endif

        mem->lambda.invoke();
        mem->lambda.release_bind_args();

        if (mem->lambda.is_detached()){
            inc::mfree_aligned(mem);

            #if xis_windows
                CloseHandle(handler);
            #endif
        }
        else{
            #if xis_windows
                ReleaseSemaphore(mem->sem_for_join, 1, nullptr);
            #endif
        }
        
        #if xis_windows
            return DWORD{};
        #elif xis_linux
            return voidp{};
        #else
            #error ""
        #endif
    }

    enum{ aligned_stack_size = 64 * 1024 };

    void helper::thread_create(thread_local_layout ** mem_ptr, clambda const & lambda){
        auto & mem                  = mem_ptr[0];
        auto   create_fail          = false;
        mem                         = (tllp)inc::malloc_aligned(sizeof(thread_local_layout) + lambda.args_bytes(), 64);

        if (mem == nullptr){
            return;
        }

        mem->lambda                 = lambda;

        #if xis_windows
            mem->sem_for_join       = CreateSemaphoreA(nullptr, 0/*初始值*/, 1/*最大值*/, nullptr);
            mem->handler            = CreateThread(nullptr, aligned_stack_size, & thread_entry, mem, 0/*立即运行*/, nullptr);
            create_fail             = mem->handler == INVALID_HANDLE_VALUE;

            if (create_fail){
                CloseHandle(mem->sem_for_join);
            }
        #else
            auto conf               = pthread_attr_t{};
            lambda.keep_args_copy_to(mem->lambda_buf);

            pthread_attr_init(& conf);
            pthread_attr_setdetachstate(& conf, lambda.is_detached());
            pthread_attr_setstacksize(& conf, aligned_stack_size);

            create_fail             = pthread_create(& mem->handler, & conf, & thread_entry, mem) != 0;

        #endif

        if (create_fail){
            mem->lambda.release_bind_args();
            inc::mfree_aligned(mem);
            mem                     = nullptr;
            return;
        }
        if (mem->lambda.is_detached()){
            mem                     = nullptr;
        }
    }
}

namespace mixc::concurrency_thread::origin{
    thread::~thread(){
        auto h = inc::atom_swap<tllp>(xref the.mem, nullptr);

        if (h == nullptr){
            return;
        }

        #if xis_windows
            DWORD ret_code; 
            WaitForSingleObject(h->sem_for_join, INFINITE);

            while(GetExitCodeThread(h->handler, & ret_code) == STILL_ACTIVE){
                inc::thread_self::yield();
            }
            CloseHandle(h->handler);
        #elif xis_linux
            pthread_join(h->handler, nullptr);
        #endif

        inc::mfree_aligned(h);
    }
}
