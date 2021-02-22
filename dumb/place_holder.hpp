#ifndef xpack_dumb_place_holder
#define xpack_dumb_place_holder
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_place_holder::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_place_holder{
    template<uxx i_v, bool hidden_v = false>
    struct place_holder{
        constexpr place_holder(){}
        enum{ index = i_v };
    };

    template<uxx i_v>
    struct place_holder<i_v, true> {};
}

#endif

xexport(mixc::dumb_place_holder::place_holder)