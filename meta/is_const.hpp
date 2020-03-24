#ifndef xpack_meta_is_const
#define xpack_meta_is_const
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_const
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_const{
        namespace inner_is_const{
            template<class a> struct meta {
                static constexpr bool result = false;
            };

            template<class a> struct meta<const a> {
                static constexpr bool result = true;
            };
        }

        template<class a>
        constexpr bool is_const = inner_is_const::meta<a>::result;
    }

#endif

namespace xuser::inc{
    using mixc::meta_is_const::is_const;
}
