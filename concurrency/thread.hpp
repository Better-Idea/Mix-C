#ifndef xpack_concurrency_thread
#define xpack_concurrency_thread
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_thread::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/move.hpp"
#include"interface/can_callback.hpp"
#include"macro/xcstyle.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"macro/xstruct.hpp"
#include"math/min.hpp"
#include"meta/function.hpp"
#include"meta/is_ptr.hpp"
#include"meta/is_same.hpp"
#include"meta/is_empty_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread{
    struct thread_local_layout;

    template<class lambda_t>
    struct lambda_joinable : lambda_t{
        static constexpr bool is_joinable = true;
        static constexpr bool is_detached = false;

        lambda_joinable(lambda_t const & lambda) : 
            lambda_t(lambda){
        }
    };
    
    template<class lambda_t>
    struct lambda_detached : lambda_t{
        static constexpr bool is_joinable = false;
        static constexpr bool is_detached = true;

        lambda_detached(lambda_t const & lambda) : 
            lambda_t(lambda){
        }
    };

    struct clambda{
        clambda(){}

        template<class lambda_t>
        requires(inc::can_callback<lambda_t, void()>)
        clambda(lambda_t const & lambda, bool is_detached) : im_detached(is_detached){
            struct closure{
                static void copy(voidp local, voidp lambda){
                    xnew(local) lambda_t(
                        inc::move(*(lambda_t *)lambda)
                    );
                }

                static void call(voidp lambda){
                    ((lambda_t *)lambda)->operator()();
                }

                static void release(voidp lambda){
                    ((lambda_t *)lambda)->~lambda_t();
                }
            };
            this->lambda    = & const_cast<lambda_t &>(lambda);
            this->copy      = & closure::copy;
            this->call      = & closure::call;
            this->release   = & closure::release;
        }

        void invoke() const {
            call(lambda);
        }

        void release_bind_args() const {
            release(lambda);
        }

        void keep_args_copy_to(voidp local) const {
            copy(local, lambda);
        }

        bool is_detached() const {
            return im_detached;
        }
    private:
        bool   im_detached;
        voidp  lambda;
        void(* copy)   (voidp local, voidp lambda);
        void(* call)   (voidp lambda);
        void(* release)(voidp lambda);
    };

    template<bool is_detached>
    struct sugar{
        template<class lambda_t>
        clambda operator *(lambda_t const & lambda){
            return { lambda, is_detached };
        }
    };

    struct helper{
        static void thread_create(thread_local_layout ** mem, clambda const & lambda);
    };
}

namespace mixc::concurrency_thread::origin{
    xstruct(
        xname(thread),
        xpubb(inc::disable_copy),
        xprif(mem, thread_local_layout *)
    )
        friend helper;

        thread() : mem(nullptr) {}

        thread(clambda const & lambda){
            helper::thread_create(xref mem, lambda);
        }

       ~thread();
    $
}

#define xjoinable           ::mixc::concurrency_thread::sugar<false>() * [=]() mutable
#define xdetached           ::mixc::concurrency_thread::sugar<true >() * [=]() mutable
#endif

xexport_space(mixc::concurrency_thread::origin)
