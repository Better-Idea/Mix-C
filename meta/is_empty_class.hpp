#ifndef xpack_meta_is_empty_class
#define xpack_meta_is_empty_class
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_is_empty_class
    #include"define/base_type.hpp"

    namespace xuser{
        template<class a> struct meta {
        private:
            static auto invoke(){
                if constexpr (__is_class(a)){
                    return *(a *)nullptr;
                }
                else{
                    struct solid_class{
                        double item;
                    };
                    return solid_class();
                }
            }

            struct test : decltype(meta<a>::invoke()){
                char dummy;
            };
        public:
            static constexpr bool result = sizeof(test) == 1;
        };

        template<class a>
        constexpr bool is_empty_class = meta<a>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_is_empty_class::is_empty_class;
}
