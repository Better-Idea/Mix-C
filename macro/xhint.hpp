#ifndef xpack_macro_xhint
#define xpack_macro_xhint
#pragma warning(disable:4390)
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xhint::inc
#include"macro/private/log.hpp"
#pragma pop_macro("xuser")

#define xhint(...)                                                              \
    ([](auto func, auto const & ... args){                                      \
        using namespace ::mixc::macro_private_log::origin;                      \
        log(for_hint, __FILE__, __LINE__, func,                                 \
            #__VA_ARGS__ ",", normal, args...                                   \
        );                                                                      \
    })(__func__, __VA_ARGS__)

#endif
