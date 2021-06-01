#ifndef xpack_concurrency_lock_state_t
#define xpack_concurrency_lock_state_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_state_t::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_state_t{
    enum class lock_state_t{
        accept,
        blocked,
    };
}

#endif

xexport(mixc::concurrency_lock_state_t::lock_state_t)