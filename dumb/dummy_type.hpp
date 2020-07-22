#ifndef xpack_dumb_dummy_type
#define xpack_dumb_dummy_type
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::dumb_dummy_type
    #include"mixc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_dummy_type{
        xstruct(
            xname(dummy_type)
        )
            template<class ... args>
            constexpr dummy_type(args const & ...){}
        $
    }

#endif

namespace xuser::inc{
    using ::mixc::dumb_dummy_type::dummy_type;
}
