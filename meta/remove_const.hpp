#ifndef xpack_meta_remove_const
#define xpack_meta_remove_const
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_remove_const
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a>
        struct meta {
            using result = a;
        };

        template<class a>
        struct meta<const a> {
            using result = a;
        };

        template<class type>
        using remove_const = typename meta<type>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_remove_const::remove_const;
}
