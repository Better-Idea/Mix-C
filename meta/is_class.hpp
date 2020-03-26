#ifndef xpack_meta_is_class
#define xpack_meta_is_class
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_class
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_class{
        template<class a>
        constexpr bool is_class = __is_class(a);
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_is_class::is_class;
}
