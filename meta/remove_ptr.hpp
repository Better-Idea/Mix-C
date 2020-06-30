#ifndef xpack_meta_remove_ptr
#define xpack_meta_remove_ptr
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_remove_ptr
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a>
        struct meta {
            using result = a;
        };

        template<class a>
        struct meta<a *> {
            using result = a;
        };

        template<class a>
        struct meta<a * const> {
            using result = a;
        };

        template<class type>
        using remove_ptr = typename meta<type>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_remove_ptr::remove_ptr;
}
