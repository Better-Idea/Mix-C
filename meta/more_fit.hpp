#ifndef xpack_meta_more_fit
#define xpack_meta_more_fit
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_more_fit
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_more_fit{
        template<uxx count, class source, class ... target> struct meta;
        template<uxx count, class source, class first, class ... target>
        struct meta<count, source, first, target...> : meta<count, source, target...>{
            static constexpr uxx index(first){
                return count - sizeof...(target) - 1;
            }
            using meta<count, source, target...>::index;
        };

        template<uxx count, class source>
        struct meta<count, source>{
            static constexpr uxx index(...){
                return not_exist;
            }
        };

        template<class source, class first, class ... target>
        constexpr uxx more_fit = meta<1 + sizeof...(target), source, first, target...>::index(source());
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_more_fit::more_fit;
}
