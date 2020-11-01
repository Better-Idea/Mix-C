#ifndef xpack_meta_ctr_cassert
#define xpack_meta_ctr_cassert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_ctr_cassert::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_ctr_cassert{
    namespace inner_cassert{
        template<bool value>
        struct meta{
            static_assert(value);
            static constexpr bool result = value;
        };
    }

    template<bool value>
    constexpr bool cassert = inner_cassert::meta<value>::result;
}

#endif

xexport(mixc::meta_ctr_cassert::cassert)