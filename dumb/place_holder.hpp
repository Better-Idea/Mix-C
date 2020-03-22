#ifndef xpack_dumb_place_holder
#define xpack_dumb_place_holder
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_place_holder
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_place_holder{
        template<uxx i>
        struct place_holder{
            static constexpr int index = i;
        };
    }

#endif

namespace xuser::inc{
    using namespace mixc::dumb_place_holder;
}
