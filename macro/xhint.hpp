#ifndef xpack_macro_xhint
#define xpack_macro_xhint
#pragma warning(disable:4390)
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xhint
#include"macro/private/log.hpp"
#pragma pop_macro("xuser")

#define xhint(...) {                                    \
    using namespace ::mixc::macro_private_log;          \
    origin::log(origin::for_hint, __FILE__, __LINE__,   \
        __func__, #__VA_ARGS__ ",", __VA_ARGS__);       \
}
#endif
