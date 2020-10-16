#ifndef xpack_meta_is_origin_array
#define xpack_meta_is_origin_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_is_origin_array
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_is_origin_array{
    template<class a>
    struct meta{ enum { result = false }; };

    template<class a, uxx length>
    struct meta<a[length]>{ enum { result = true }; };

    template<class a>
    concept is_origin_array = meta<a>::result;
}

#endif

namespace xuser::inc{
    using ::mixc::meta_is_origin_array::is_origin_array;
}
