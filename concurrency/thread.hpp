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
#include"meta/is_empty_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_thread{
    struct thread_local_layout;

    struct clambda{
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

            this->lambda_arg_size   = inc::is_empty_class<lambda_t> ? 0 : sizeof(lambda_t);
            this->lambda            = & const_cast<lambda_t &>(lambda);
            this->copy              = & closure::copy;
            this->call              = & closure::call;
            this->release           = & closure::release;
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

        uxx args_bytes() const {
            return lambda_arg_size;
        }
    private:
        bool   im_detached;
        u16    lambda_arg_size;
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

        thread(thread && self) : 
            mem(inc::atom_swap<thread_local_layout *>(xref self.mem, nullptr)) {
        }

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
