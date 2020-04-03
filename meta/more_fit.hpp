#ifndef xpack_meta_more_fit
#define xpack_meta_more_fit
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_more_fit
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_more_fit{
        template<uxx count, class source, class ... target> struct meta;
        template<uxx count, class source, class first, class ... target>
        struct meta<count, source, first, target...> : meta<count, source, target...>{
            static auto test(first){
                struct inner { u08 mem[count - sizeof...(target)]; };
                return inner();
            }
            using meta<count, source, target...>::test;
        };

        template<uxx count, class source>
        struct meta<count, source>{
            static constexpr u08 test(...){
                return u08();
            }
        };

        template<class source, class first, class ... target>
        constexpr uxx more_fit = sizeof(
            decltype(
                meta<1 + sizeof...(target), source, first, target...>::test(*(source *)nullptr)
            )
        ) - 1;
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_more_fit::more_fit;
}
