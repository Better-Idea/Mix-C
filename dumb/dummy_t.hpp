#pragma once
#include"meta_seq/tlist.hpp"

namespace mixc{
    struct dummy_t{
        template<class ... args>
        dummy_t(args const & ...){}
        using member_list = mixc::tlist<>;
    };
}

#ifndef xpack_dumb_dummy_t
#define xpack_dumb_dummy_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_dummy_t
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_dummy_t{
        struct dummy_t{
            template<class ... args>
            constexpr dummy_t(args const & ...){}
            using member_list = mixc::tlist<>;
        };
    }

#endif

namespace xuser::inc{
    using namespace mixc::dumb_dummy_t;
}
