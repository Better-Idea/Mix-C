#ifndef xpack_meta_is_based_on
#define xpack_meta_is_based_on
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_based_on
        #include"define/base_type.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_based_on{
        template<class father, class child>
        constexpr bool is_based_on = __is_base_of(father, child);
    }

#endif

namespace xuser::inc{
    using namespace mixc::meta_is_based_on;
}
