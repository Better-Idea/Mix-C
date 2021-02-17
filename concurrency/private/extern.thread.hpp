#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"macro/xalign.hpp"
#include"memory/private/tiny_allocator.hpp"
#include<pthread.h>

namespace mixc::concurrency_thread{
    typedef struct thread_local_layout{
        clambda                 lambda;
        pthread_t               handler;

        xalign(64)
        u08                     lambda_buf[1];
    } * tllp;

    static inline tllp          free_list;

    inline voidp thread_entry(voidp ptr){
        // 使用原子操作保证其他 cpu 线程可见
        auto mem            = static_cast<tllp>(ptr);
        mem->lambda.invoke();
        mem->lambda.release_bind_args();
        
        if (mem->lambda.is_detached()){
            inc::mfree(mem);
        }
        return nullptr;
    }

    enum{ aligned_stack_size = 64 * 1024 };

    void helper::thread_create(thread_local_layout ** mem_ptr, clambda const & lambda){
        auto   conf             = pthread_attr_t{};
        auto & mem              = mem_ptr[0];

        mem                     = (tllp)inc::malloc(sizeof(thread_local_layout) + lambda.args_bytes());

        if (mem == nullptr){
            return;
        }

        mem->lambda             = lambda;
        lambda.keep_args_copy_to(mem->lambda_buf);

        pthread_attr_init(& conf);
        pthread_attr_setdetachstate(& conf, lambda.is_detached());
        pthread_attr_setstacksize(& conf, aligned_stack_size);

        if (pthread_create(& mem->handler, & conf, & thread_entry, mem) < 0){
            mem->lambda.release_bind_args();
            inc::mfree(mem);
            mem                 = nullptr;
            return;
        }
        if (mem->lambda.is_detached()){
            mem                 = nullptr;
        }
    }
}

namespace mixc::concurrency_thread::origin{
    thread::~thread(){
        if (auto h = inc::atom_swap<tllp>(xref the.mem, nullptr); h != nullptr){
            auto v = pthread_join(pthread_t(h->handler), nullptr);
            inc::mfree(mem);
        }
    }
}
