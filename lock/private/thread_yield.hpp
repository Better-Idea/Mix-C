#ifndef xpack_lock_private_thread_yield
#define xpack_lock_private_thread_yield

namespace mixc::lock_private_thread_yield{
    void thread_yield();
}

#endif

namespace xuser{
    using ::mixc::lock_private_thread_yield::thread_yield;
}
