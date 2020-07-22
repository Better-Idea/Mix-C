#ifndef xpack_meta_remove_ref
#define xpack_meta_remove_ref
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_remove_ref
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_remove_ref{
    template<class a>
    struct meta {
        using result = a;
    };

    template<class a>
    struct meta<a &> {
        using result = a;
    };

    template<class type>
    using remove_ref = typename meta<type>::result;
}

#endif

namespace xuser::inc{
    using ::mixc::meta_remove_ref::remove_ref;
}
