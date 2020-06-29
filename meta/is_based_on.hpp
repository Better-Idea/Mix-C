#ifndef xpack_meta_is_based_on
#define xpack_meta_is_based_on
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_based_on
    #include"define/base_type.hpp"
    #include"meta/is_same.hpp"

    namespace xuser{
        template<class father, class child>
        constexpr bool is_based_on = __is_base_of(father, child);
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_based_on::is_based_on;
}
