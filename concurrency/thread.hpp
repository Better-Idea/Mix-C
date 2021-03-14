#ifndef xpack_concurrency_thread
#define xpack_concurrency_thread
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_thread::inc
#include"concurrency/lock/atom_swap.hpp"
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/move.hpp"
#include"interface/can_callback.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"macro/xstruct.hpp"
#include"memory/allocator.hpp"
#include"meta/is_empty_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread{
    using lambda_call = void(*)(voidp lambda);

    xstruct(
        xname(clambda_meta),
        xpubf(bytes,        uxx),
        xpubf(is_detached,  bool),
        xpubf(call,         lambda_call),
        xpubf(release,      lambda_call),
        xpubf(sem,          voidp),
        xpubf(handler,      voidp)

        // this + 1 定位到 lambda 参数的偏移
    ) $

    xstruct(
        xname(clambda),
        xprif(plambda, clambda_meta *)
    )
        clambda() : plambda(nullptr){}
        clambda(clambda const &) = default;

        template<class lambda_t>
        requires(inc::can_callback<lambda_t, void()>)
        clambda(lambda_t const & lambda, bool is_detached){
            struct closure{
                using lp = lambda_t *;

                static void call(voidp lambda){
                    lp(lambda)->operator()();
                }

                static void release(voidp lambda){
                    lp(lambda)->~lambda_t();
                }
            };

            auto args_bytes         = inc::is_empty_class<lambda_t> ? sizeof(lambda_t) : 0;
            auto total_bytes        = inc::memory_size{ args_bytes + sizeof(clambda_meta) };

            if (plambda = inc::alloc<clambda_meta>(total_bytes);
                plambda == nullptr){
                im_initialize_fail();
                return;
            }

            xnew(plambda + 1) lambda_t(
                inc::move(lambda)
            );

            plambda->bytes          = total_bytes;
            plambda->is_detached    = is_detached;
            plambda->call           = & closure::call;
            plambda->release        = & closure::release;
            plambda->sem            = nullptr;
            plambda->handler        = nullptr;
        }

        void invoke() const {
            auto call               = plambda->call;
            auto args               = plambda + 1;
            call(args);
        }

        void release() const {
            auto call               = plambda->release;
            auto args               = plambda + 1;
            call(args);
            inc::free(plambda, inc::memory_size{plambda->bytes});
            plambda                 = nullptr;
        }

        bool is_detached() const {
            return plambda->is_detached;
        }

        void semaphore_for_join(voidp value){
            plambda->sem            = value;
        }

        voidp semaphore_for_join() const {
            return plambda->sem;
        }

        void handler(voidp value){
            plambda->handler        = value;
        }

        voidp handler() const {
            return plambda->handler;
        }

        bool is_initialize_fail() const {
            return uxx(plambda) == not_exist;
        }

        void im_initialize_fail() const {
            // 不提供 is_initialize_fail(false) 接口
            // 会覆盖 clambda::plambda 原有指针的内容
            plambda                 = (clambda_meta *)(not_exist);
        }

        bool is_valid() const {
            return plambda != nullptr && uxx(plambda) != not_exist;
        }
    $

    template<bool is_detached_v>
    struct sugar{
        template<class lambda_t>
        clambda operator *(lambda_t const & lambda){
            return { lambda, is_detached_v };
        }
    };
}

namespace mixc::concurrency_thread::origin{
    xstruct(
        xname(thread),
        xpubb(inc::disable_copy),
        xprif(plambda, clambda)
    )
        thread()                = default;
        thread(thread && self)  = default;
        thread(clambda && lambda);
       ~thread();

        bool is_initialize_fail() const {
            return plambda.is_initialize_fail();
        }
    $
}

#define xjoinable           ::mixc::concurrency_thread::sugar<false>() * [=]() mutable
#define xdetached           ::mixc::concurrency_thread::sugar<true >() * [=]() mutable
#endif

xexport_space(mixc::concurrency_thread::origin)
