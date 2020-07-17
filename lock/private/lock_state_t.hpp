#ifndef xpack_lock_private_lock_state_t
#define xpack_lock_private_lock_state_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lock_private_lock_state_t
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::lock_private_lock_state_t{
    enum class lock_state_t{
        accept,
        blocked,
    };
}

#endif

namespace xuser::inc{
    using ::mixc::lock_private_lock_state_t::lock_state_t;
}
