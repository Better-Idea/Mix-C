#pragma once

namespace mixc{
    namespace inner_is_empty_class{
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

    }

    template<class a>
    constexpr bool is_empty_class = mixc::inner_is_empty_class::meta<a>::result;
}
