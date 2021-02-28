#ifndef xpack_concurrency_thread_self
#define xpack_concurrency_thread_self
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_thread_self::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread_self::origin{
    struct thread_self{
        thread_self() = delete;
        static void sleep(uxx millisecond);
        static void yield();
    };
}

#endif

xexport_space(mixc::concurrency_thread_self::origin)
