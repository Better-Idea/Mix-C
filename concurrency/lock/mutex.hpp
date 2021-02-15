#ifndef xpack_lock_mutex
#define xpack_lock_mutex
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_mutex::inc
#include"define/base_type.hpp"
#include"concurrency/lock/atom_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/lock/lock_state_t.hpp"
#include"concurrency/thread_self.hpp"
#include"macro/xexport.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_mutex::origin{
    using inc::lock_state_t;

    xstruct(
        xname(mutex),
        xprif(item,  bool)
    )
        mutex() : item(false){}

        lock_state_t try_lock() const {
            return inc::atom_swap(xref item, true) == true ?
                lock_state_t::blocked : lock_state_t::accept;
        }

        void lock() const {
            while(try_lock() == lock_state_t::blocked){
                inc::thread_self::yield();
            }
        }

        void unlock() const {
            inc::atom_swap(xref item, false);
        }

        template<class callback>
        void lock(callback const & call) const {
            lock();
            call();
            unlock();
        }

        // 兼容 meta/is_builtin_lock 的接口
        template<auto opr, class this_t, class callback>
        void lock(this_t, callback const & call) const {
            lock(call);
        }

        template<class type>
        static lock_state_t try_lock(type * field, uxx index){
            using ut = inc::unsigned_type<type>;
            ut mask  = ut(1) << index;
            return inc::atom_fetch_or<ut>((ut *)field, mask) & mask ?
                lock_state_t::blocked : lock_state_t::accept;
        }

        template<class type>
        static void unlock(type * field, uxx index){
            using ut = inc::unsigned_type<type>;
            ut mask  = ~(ut(1) << index);
            inc::atom_and<ut>((ut *)field, mask);
        }

        template<class type, class callback>
        static void lock(type * field, uxx index, callback const & call){
            while(try_lock(field, index) == lock_state_t::blocked){
                inc::thread_self::yield();
            }
            call();
            unlock(field, index);
        }
    $
}

#endif

xexport_space(mixc::lock_mutex::origin)