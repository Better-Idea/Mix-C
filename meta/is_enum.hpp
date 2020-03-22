#ifndef xpack_meta_is_enum
#define xpack_meta_is_enum
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_enum
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_enum{
        template<class a>
        constexpr bool is_enum = __is_enum(a);
    }

#endif

namespace xuser::inc{
    using namespace mixc::meta_is_enum;
}
