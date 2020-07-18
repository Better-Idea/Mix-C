#ifndef xpack_lock_mutex
#define xpack_lock_mutex
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_mutex
#include"lock/atom_swap.hpp"
#include"lock/private/lock_state_t.hpp"
#include"lock/private/thread_yield.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_mutex::origin{
    using inc::lock_state_t;

    xstruct(
        xname(mutex),
        xprif(item, bool)
    )
        mutex() : item(false){}

        lock_state_t try_lock(){
            return inc::atom_swap(xref item, true) == true ?
                lock_state_t::blocked : lock_state_t::accept;
        }

        void lock(){
            while(try_lock() == lock_state_t::blocked){
                inc::thread_yield();
            }
        }

        void unlock(){
            inc::atom_swap(xref item, false);
        }

        template<class callback>
        void lock(callback const & call){
            lock();
            call();
            unlock();
        }
    $
}

#endif

namespace xuser::inc{
    using namespace ::mixc::lock_mutex::origin;
}
