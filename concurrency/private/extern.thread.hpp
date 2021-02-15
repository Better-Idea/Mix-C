#ifdef  xuser
#undef  xuser
#endif

#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"memory/private/tiny_allocator.hpp"
#include<pthread.h>
#include<time.h>
#include<unistd.h>

namespace mixc::concurrency_thread{
    typedef struct thread_local_layout{
        thread_local_layout *   next;
        uxx                     handler;
        origin::thread_routine  routine;
        voidp                   args;
    } * tllp;

    inline voidp thread_entry(voidp ptr){
        auto routine    = tllp(ptr)->routine;
        auto args       = tllp(ptr)->args;
        return routine(args);
    }

    enum{ aligned_stack_size = 64 * 1024 };
}

namespace mixc::concurrency_thread::origin{
    extern uxx min_priority_of(thread_policy_t method){
        return sched_get_priority_min((int)(method));
    }
    extern uxx max_priority_of(thread_policy_t method){
        return sched_get_priority_max((int)(method));
    }

    thread::thread(thread_args ta){
        auto tid                = pthread_t{};
        auto prior              = sched_param{ (int)ta.priority() };
        auto conf               = pthread_attr_t{};
        auto bytes              = aligned_stack_size;
        the.mem                 = static_cast<tllp>(inc::malloc_aligned(bytes, aligned_stack_size));
        the.mem->routine        = ta.routine();
        the.mem->args           = ta.args();

        pthread_attr_setschedpolicy(& conf, (int)ta.policy());
        pthread_attr_setschedparam (& conf, & prior);
        pthread_attr_setdetachstate(& conf, ta.is_detach());
        pthread_attr_setstack      (& conf, the.mem, bytes);

        if (pthread_create(& tid, & conf, & thread_entry, the.mem) < 0){
            inc::mfree_aligned(the.mem);
            the.mem             = nullptr;
            return;
        }
        if (not ta.is_detach()){
            the.mem->handler    = uxx(tid);
        }
    }

    thread::~thread(){
        if (auto h = inc::atom_swap<tllp>(xref the.mem, nullptr); h != nullptr){
            pthread_join(pthread_t(h->handler), nullptr);
            inc::mfree_aligned(h);
        }
    }
}
