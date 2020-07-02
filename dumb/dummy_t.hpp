#ifndef xpack_dumb_dummy_t
#define xpack_dumb_dummy_t
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::dumb_dummy_t
    #include"mixc.hpp"

    namespace xuser{
        struct dummy_t {
            xgc_fields(
                xiam(dummy_t)
            );
        public:
            template<class ... args>
            constexpr dummy_t(args const & ...){}
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::dumb_dummy_t::dummy_t;
}
