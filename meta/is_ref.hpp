#ifndef xpack_meta_is_ref
#define xpack_meta_is_ref
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_is_ref
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_is_ref{
        namespace inner_is_ref{
            template<class a> struct meta {
                static constexpr bool result = false;
            };

            template<class a> struct meta<a &> {
                static constexpr bool result = true;
            };
        }

        template<class a>
        constexpr bool is_ref = inner_is_ref::meta<a>::resuslt;
    }

#endif

namespace xuser::inc{
    using namespace mixc::meta_is_ref;
}
