#ifndef xpack_concurrency_thread_self
#define xpack_concurrency_thread_self
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_thread_self::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"memop/cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread_self::origin{
    struct thread_self{
        thread_self() = delete;

        static bool is_main_thread(){
            return id() == 0;
        }

        static uxx  id();
        static void resume(uxx thread_id);
        static void suspend(uxx timeout_ms);
        static void suspend(){
            uxx until_resume = uxx(-1);
            suspend(until_resume);
        }
        static void sleep(uxx millisecond);
        static void yield();
        static void gc_sync();
    };
}

#endif

xexport_space(mixc::concurrency_thread_self::origin)
