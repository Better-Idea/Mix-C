#ifndef xpack_dumb_dummy_t
#define xpack_dumb_dummy_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_dummy_t
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_dummy_t{
        xgc(dummy_t)
            xgc_fields();

            template<class ... args>
            constexpr dummy_t(args const & ...){}
        xgc_end();
    }

#endif

namespace xuser::inc{
    using mixc::dumb_dummy_t::dummy_t;
}
