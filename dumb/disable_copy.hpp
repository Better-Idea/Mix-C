#ifndef xpack_dumb_disable_copy
#define xpack_dumb_disable_copy
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::dumb_disable_copy
    #include"mixc.hpp"

    namespace xuser::origin {
        struct disable_copy {
            xgc_fields(
                xiam(disable_copy)
            );
        public:
            disable_copy() = default;
            disable_copy(disable_copy const &) = delete;
            void operator=(disable_copy const &) = delete;
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc {
    using ::mixc::dumb_disable_copy::origin::disable_copy;
}
