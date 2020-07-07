#ifndef xpack_meta_has_operator_cast
#define xpack_meta_has_operator_cast
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_has_operator_cast
    #include"configure.hpp"
    #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_has_operator_cast{
        template<class target, class source>
        concept has_operator_cast = requires(source value){
            value.operator target();
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_has_operator_cast::has_operator_cast;
}
