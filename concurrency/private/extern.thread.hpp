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
#include<bits/local_lim.h>
#include<pthread.h>
#define handler_t       pthread_t
#endif

namespace mixc::concurrency_thread{
    typedef struct thread_local_layout{
        clambda                 lambda;
        handler_t               handler;
        handler_t               sem_for_join;
        uxx                     rbp;

        xalign(64)
        u08                     lambda_buf[1];
    } * tllp;

    enum{ aligned_stack_size = 64 * 1024 };

    static void thread_entry_core(tllp mem_old){
        #if xis_x86
            #if xis_os32
                #define xrbp    "%ebp"
                #define xrsp    "%esp"
            #else
                #define xrbp    "%rbp"
                #define xrsp    "%rsp"
            #endif
        #elif xis_arm
            #define xrbp        "%sp"
        #endif

        #ifdef xrsp
            asm("push " xrsp);
        #endif

        auto new_sp     = u08p(mem_old) + aligned_stack_size - sizeof(voidp) * 64;
        asm("mov %" xrbp ", %0":"=m"(mem_old->rbp));
        asm("mov %0, %" xrbp ""::"r"(new_sp));

        uxx  sp         = uxx(& sp) & ~(aligned_stack_size - 1);
        tllp mem        = tllp(sp);
        auto end        = u08p(mem) + aligned_stack_size - sizeof(voidp) * 64;

        mem->lambda.invoke();
        mem->lambda.release_bind_args();

        asm("mov %0, %" xrbp::"r"(mem->rbp));
        
        #ifdef xrsp
            asm("pop " xrsp);
        #endif
    }

    inline auto thread_entry(voidp ptr){
        // 使用原子操作保证其他 cpu 线程可见
        auto mem            = tllp(ptr);
        auto handler        = mem->handler = 
        
        #if xis_windows
            GetCurrentThread();
        #elif xis_linux
            pthread_self();
        #endif

        thread_entry_core(mem);

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

    void helper::thread_create(thread_local_layout ** mem_ptr, clambda const & lambda){
        auto & mem                  = mem_ptr[0];
        auto   create_fail          = false;
        mem                         = (tllp)inc::malloc_aligned(aligned_stack_size, aligned_stack_size);

        if (mem == nullptr){
            return;
        }

        mem->lambda                 = lambda;

        #if xis_windows
            mem->sem_for_join       = CreateSemaphoreA(nullptr, 0/*初始值*/, 1/*最大值*/, nullptr);
            mem->handler            = CreateThread(nullptr, 16 * 1024, & thread_entry, mem, 0/*立即运行*/, nullptr);
            create_fail             = mem->handler == INVALID_HANDLE_VALUE;

            if (create_fail){
                CloseHandle(mem->sem_for_join);
            }
        #else
            auto conf               = pthread_attr_t{};
            lambda.keep_args_copy_to(mem->lambda_buf);

            pthread_attr_init(& conf);
            pthread_attr_setdetachstate(& conf, lambda.is_detached());
            pthread_attr_setstacksize(& conf, PTHREAD_STACK_MIN);

            create_fail             = pthread_create(& mem->handler, & conf, & thread_entry, mem) != 0;
        #endif

        if (create_fail){
            mem->lambda.release_bind_args();
            inc::mfree_aligned(mem);
            mem                     = nullptr;
            return;
        }

        // 这里不能使用 mem->lambda，因为可能已经释放了
        if (lambda.is_detached()){
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
