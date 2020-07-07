#ifndef xpack_dumb_disable_copy
#define xpack_dumb_disable_copy
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::dumb_disable_copy
    #include"mixc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_disable_copy {
        xstruct(
            xiam(disable_copy)
        )
            disable_copy() = default;
            disable_copy(disable_copy const &) = delete;
            void operator=(disable_copy const &) = delete;
        $
    }

#endif

namespace xuser::inc {
    using ::mixc::dumb_disable_copy::disable_copy;
}
