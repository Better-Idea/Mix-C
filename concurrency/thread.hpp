#ifndef xpack_concurrency_thread
#define xpack_concurrency_thread
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_thread::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"macro/xcstyle.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"math/min.hpp"
#include"meta/function.hpp"
#include"meta/is_ptr.hpp"
#include"meta/is_same.hpp"
#include"meta/is_empty_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread{
    struct thread_local_layout;

    inline voidp do_nothing(voidp){
        return nullptr;
    }
}

namespace mixc::concurrency_thread::origin{
    using thread_routine = voidp(*)(voidp args);

    enum class thread_policy_t : u08{
        other,
        fifo,
        round_robin,
    };

    enum class thread_priority_t : i08{};

    extern uxx min_priority_of(thread_policy_t method);
    extern uxx max_priority_of(thread_policy_t method);

    xstruct(
        xname(thread_args),
        xprif(proutine  , thread_routine),
        xprif(pargs     , voidp),
        xprif(pis_detach, bool),
        xprif(ppolicy   , thread_policy_t),
        xprif(ppriority , thread_priority_t)
    )
        using final_t = the_t;

        thread_args() :
            proutine  (& do_nothing),
            pargs     (nullptr),
            pis_detach(false),
            ppolicy   (thread_policy_t::other),
            ppriority (thread_priority_t{0}){
        }

        template<
            inc::is_empty_class routine_t, 
            class               func_t = decltype(& routine_t::operator()),
            class               ret_t  = typename inc::function<func_t>::return_type,
            class               arg_t  = typename inc::function<func_t>::template args<0>
        >
        requires(
            inc::is_ptr<arg_t> and (inc::is_same<ret_t, void> or inc::is_ptr<ret_t>)
        )
        final_t & routine(routine_t const &){
            struct closure{
                static voidp call(voidp args){
                    if constexpr (inc::is_same<void, ret_t>){
                        routine_t()(arg_t(args));
                        return nullptr;
                    }
                    else{
                        return routine_t()(arg_t(args));
                    }
                }
            };
            the.proutine = & closure::call;
            return thex;
        }

        final_t & sched(
            thread_policy_t     method, 
            thread_priority_t   priority = thread_priority_t{0}) const{

            auto min_prior  = min_priority_of(method);
            auto max_prior  = max_priority_of(method);
            the.ppolicy     = method;
            the.ppriority   = static_cast<thread_priority_t>(
                inc::min(max_prior, min_prior + uxx(priority))
            );
            return thex;
        }

        xpubget_pubset(routine  )
        xpubget_pubset(args     )
        xpubget_pubset(is_detach)
        xpubget(policy)
        xpubget(priority)

    $

    xstruct(
        xname(thread),
        xpubb(inc::disable_copy),
        xprif(mem, thread_local_layout *)
    )
        thread() : mem(nullptr) {}
        thread(thread_args ta);
       ~thread();
    $
}

#endif

xexport_space(mixc::concurrency_thread::origin)
