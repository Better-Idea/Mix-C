#ifndef xpack_meta_is_union
#define xpack_meta_is_union
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_union
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_union{
        template<class a>
        constexpr bool is_union = __is_union(a);
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_is_union::is_union;
}
