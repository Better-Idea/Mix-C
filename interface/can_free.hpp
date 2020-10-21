#ifndef xpack_interface_can_free
#define xpack_interface_can_free
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_free
#include"define/base_type.hpp"
#include"interface/can_callback.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_free{
    template<class item_t>
    using can_free = inc::can_callback<void(item_t *, uxx)>;
}

#endif

namespace xuser::inc{
    using ::mixc::interface_can_free::can_free;
}
