#ifndef xpack_lock_builtin_lock
#define xpack_lock_builtin_lock
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_builtin_lock::inc
#include"define/base_type.hpp"
#include"concurrency/lock/mutex.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_builtin_lock::origin{
    xstruct(
        xname(with_lwmutex)
    )
        constexpr with_lwmutex(){}

        template<auto opr_v, class this_t, class invoke_t>
        void lock(this_t * ptr, invoke_t const & call) const {
            inc::mutex::lock(ptr, 0, call); // 第 0 位作为互斥位
        }
    $

    xstruct(
        xname(lock_free)
    )
        constexpr lock_free(){}

        template<auto opr_v, class this_t, class callback>
        void lock(this_t, callback const & call) const {
            call();
        }
    $
}

#endif

xexport_space(mixc::lock_builtin_lock::origin)