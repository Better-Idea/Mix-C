#ifndef xpack_meta_has_cast
#define xpack_meta_has_cast
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_has_cast
        #include"configure.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_has_cast{
        #if xfor_msvc_hint
            template<class target, class source>
            struct meta{
            private:
                static u08 test(target){
                    return true;
                }
                static u16 test(...){
                    return false;
                }
            public:
                static constexpr bool result = sizeof(
                    decltype(test(*(source *)nullptr))
                ) == 1;
            };

            template<class target, class source>
            constexpr bool has_cast = meta<target, source>::result;
        #else
            template<class target, class source>
            concept has_cast = requires(target tar, source src){
                tar = src;
            };
        #endif
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_has_cast::has_cast;
}
