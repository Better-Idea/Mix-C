#include<thread>
#ifdef xuser
    #undef xuser
#endif

namespace mixc::lock_private_thread_yield{
    void thread_yield(){
        std::this_thread::yield();
    }
}
