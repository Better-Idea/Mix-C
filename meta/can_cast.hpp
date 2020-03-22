#ifndef xpack_meta_can_cast
#define xpack_meta_can_cast
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_can_cast
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_can_cast{
        namespace inner_can_cast{
            template<class source, class target>
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
        }

        template<class source, class target>
        constexpr bool can_cast = inner_can_cast::meta<source, target>::result;
    }

#endif

namespace xuser::inc{
    using namespace mixc::meta_can_cast;
}
