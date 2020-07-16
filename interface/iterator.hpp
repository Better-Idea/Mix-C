#ifndef xpack_interface_iterator
#define xpack_interface_iterator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_iterator
#include"define/base_type.hpp"
#include"interface/can_callback.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_iterator::origin{
    template<class a0, class ... args>
    using iterator = inc::can_callback<void(uxx index, a0, args...)>;

    template<class a0, class ... args>
    using iteratorx = inc::can_callback<loop_t(uxx index, a0, args...)>;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::interface_iterator::origin;
}
