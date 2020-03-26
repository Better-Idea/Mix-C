#ifndef xpack_meta_is_same
#define xpack_meta_is_same
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_same
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_same{
        namespace inner_is_same{
        template<class a, class b> struct meta {
                static constexpr bool result = false;
            };

            template<class a> struct meta<a, a> {
                static constexpr bool result = true;
            };
        }

        template<class a, class b>
        constexpr bool is_same = inner_is_same::meta<a, b>::result;
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_is_same::is_same;
}
