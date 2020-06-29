#ifndef xpack_meta_is_enum
#define xpack_meta_is_enum
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_enum
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a>
        constexpr bool is_enum = __is_enum(a);
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_enum::is_enum;
}
