#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"macro/xalign.hpp"
#include"memory/allocator.hpp"
#include"memory/private/tiny_allocator.hpp"
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<bits/local_lim.h>

namespace mixc::concurrency_thread{
    typedef struct thread_local_layout{
        thread_local_layout *   next;
        pthread_t               handler;
        voidp                   stack;
        clambda                 lambda;

        xalign(64)
        u08                     lambda_buf[1];
    } * tllp;

    typedef struct link_node{
        link_node             * next;
    } * lnp;

    static inline pthread_mutex_t   mutex;
    static inline tllp              free_list;
    static inline uxx               count_of_detached;
    static inline bool              once = true;

    inline voidp thread_clean(voidp){
        pthread_mutex_lock(& mutex);

        for(uxx i = 0;;){
            if (inc::atom_sub<uxx>(xref count_of_detached, i) == 0){
                pthread_mutex_lock(& mutex);
            }

            tllp    top     = inc::atom_swap<tllp>(xref free_list, nullptr);
            tllp    next    = top;

            for(i = 0; top != nullptr; i++, top = next){
                pthread_join(top->handler, nullptr);
                next    = inc::atom_load(xref top->next);
                inc::mfree_aligned(top->stack);
                inc::mfree_aligned(top);
            }
        }
    }

    inline voidp thread_entry(voidp ptr){
        auto p = tllp(ptr);
        // *(voidp *)(p->stack) = p;
        p->lambda.invoke();
        p->lambda.release_bind_args();

        if (p->lambda.is_detached()){
            p->next = inc::atom_swap(xref free_list, p);
            inc::atom_add<uxx>(xref count_of_detached, 1);
            pthread_mutex_unlock(& mutex);
        }
        return nullptr;
    }

    enum{ aligned_stack_size = 64 * 1024 };

    void helper::thread_create(thread_local_layout ** mem_ptr, clambda const & lambda){
        auto   pid              = pthread_t{};
        auto   conf             = pthread_attr_t{};

        if (inc::atom_swap(xref once, false)){
            pthread_attr_init(& conf);
            pthread_attr_setstacksize(& conf, PTHREAD_STACK_MIN);
            pthread_create(& pid, & conf, & thread_clean, nullptr);
        }

        auto & mem              = mem_ptr[0];
        auto   stack_bytes      = aligned_stack_size;
        auto   stack            = inc::malloc_aligned(stack_bytes, aligned_stack_size);
        auto   mem_bytes        = sizeof(thread_local_layout);
        mem                     = static_cast<thread_local_layout *>(inc::malloc_aligned(mem_bytes, 64));
        mem->stack              = stack;
        mem->lambda             = lambda;
        lambda.keep_args_copy_to(mem->lambda_buf);

        pthread_attr_init(& conf);
        pthread_attr_setstack(& conf, stack, stack_bytes); 
        // =============================================================================================
        // 需要注意栈生长的方向，假定是向低地址生长
        // 对于分离的线程需要回收

        // 内部会清空栈区
        if (pthread_create(& mem->handler, & conf, & thread_entry, mem) < 0){
            mem->lambda.release_bind_args();
            inc::mfree_aligned(mem->stack);
            inc::mfree_aligned(mem);
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
            inc::mfree_aligned(h);
        }
    }
}
