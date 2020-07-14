#ifndef xpack_meta_is_origin_array
#define xpack_meta_is_origin_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_origin_array
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_origin_array{
    template<class a>
    constexpr bool is_origin_array = false;

    template<class item_t, uxx length>
    constexpr bool is_origin_array<item_t[length]> = true;
}

#endif

namespace xuser::inc{
    using ::mixc::meta_is_origin_array::is_origin_array;
}
