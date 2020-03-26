#ifndef xpack_dumb_place_holder
#define xpack_dumb_place_holder
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_place_holder
        #include"define/base_type.hpp"
        #include"meta_seq/vlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_place_holder{
        template<uxx i, bool hidden = false>
        struct place_holder{
            enum{
                index = i
            };
            using member_list = inc::vlist<>;
        };

        template<uxx i>
        struct place_holder<i, true> {};
    }

#endif

namespace xuser::inc{
    using ::mixc::dumb_place_holder::place_holder;
}
