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
#include"macro/xref.hpp"
#include"macro/xstruct.hpp"
#include"memop/cast.hpp"
#include"meta/is_empty_class.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread{
    using lambda_call = void(*)(voidp lambda);

    xstruct(
        xname(clambda_meta),
        xpubf(m_bytes,        uxx),
        xpubf(m_is_detached,  bool),
        xpubf(m_call,         lambda_call),
        xpubf(m_release,      lambda_call),
        xpubf(m_sem,          voidp),
        xpubf(m_mutex,        voidp),
        xpubf(m_handler,      voidp)

        // this + 1 定位到 lambda 参数的偏移
    ) $

    xstruct(
        xname(clambda),
        xprif(m_lambda, clambda_meta *)
    )
        clambda() : m_lambda(nullptr){}
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

            auto args_bytes         = inc::is_empty_class<lambda_t> ? 0 : sizeof(lambda_t);
            auto total_bytes        = inc::memory_size{ args_bytes + sizeof(clambda_meta) };

            if (m_lambda = inc::alloc<clambda_meta>(total_bytes);
                m_lambda == nullptr){
                im_initialize_fail();
                return;
            }

            xnew(m_lambda + 1) lambda_t(
                inc::move(lambda)
            );

            m_lambda->m_bytes        = total_bytes;
            m_lambda->m_is_detached  = is_detached;
            m_lambda->m_call         = & closure::call;
            m_lambda->m_release      = & closure::release;
            m_lambda->m_sem          = nullptr;
            m_lambda->m_mutex        = nullptr;
            m_lambda->m_handler      = nullptr;
        }

        void invoke() const {
            auto call               = m_lambda->m_call;
            auto args               = m_lambda + 1;
            call(args);
        }

        void release() const {
            auto call               = m_lambda->m_release;
            auto args               = m_lambda + 1;
            call(args);
            inc::free(m_lambda, inc::memory_size{m_lambda->m_bytes});
            m_lambda                 = nullptr;
        }

        bool is_detached() const {
            return m_lambda->m_is_detached;
        }

        void semaphore_for_join(voidp value){
            m_lambda->m_sem          = value;
        }

        voidp semaphore_for_join() const {
            return m_lambda->m_sem;
        }

        void semaphore_for_suspend(voidp value){
            m_lambda->m_mutex        = value;
        }

        voidp semaphore_for_suspend() const {
            return m_lambda->m_mutex;
        }

        void handler(voidp value){
            m_lambda->m_handler      = value;
        }

        voidp handler() const {
            return m_lambda->m_handler;
        }

        bool is_initialize_fail() const {
            return uxx(m_lambda) == not_exist;
        }

        void im_initialize_fail() const {
            // 不提供 is_initialize_fail(false) 接口
            // 会覆盖 clambda::m_lambda 原有指针的内容
            m_lambda                 = (clambda_meta *)(not_exist);
        }

        bool is_valid() const {
            return m_lambda != nullptr && uxx(m_lambda) != not_exist;
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
        xprif(m_lambda, clambda)
    )
        thread()                = default;
        thread(thread && self) : 
            m_lambda(inc::atom_swap(xref(self.m_lambda), clambda{})){
        }
        thread(clambda && lambda);
       ~thread();

        void operator=(thread && self){
            m_lambda = inc::atom_swap(xref(self.m_lambda), clambda{});
        }

        bool is_initialize_fail() const {
            return m_lambda.is_initialize_fail();
        }

        uxx id() const {
            return inc::cast<uxx>(m_lambda);
        }

        void resume();
    $
}

#define xjoinable           ::mixc::concurrency_thread::sugar<false>() * [=]() mutable
#define xdetached           ::mixc::concurrency_thread::sugar<true >() * [=]() mutable
#define xjoinable_global    ::mixc::concurrency_thread::sugar<false>() * []()
#define xdetached_global    ::mixc::concurrency_thread::sugar<true >() * []()
#endif

xexport_space(mixc::concurrency_thread::origin)
