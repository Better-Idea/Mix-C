#ifndef xpack_define_inf
#define xpack_define_inf
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::define_inf
        #include"define/base_type.hpp"
        #include"define/mfxx.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::define_inf{
        constexpr inc::mf64 inf_pos { 0ull, 0x7ffull, 0ull };
        constexpr inc::mf64 inf_neg { 1ull, 0x7ffull, 0ull };

        struct inf_t{
            constexpr inf_t(){}

            template<class type>
            friend bool operator == (type const & value, inf_t){
                return value == inf_pos or value == inf_neg;
            }

            template<class type>
            friend bool operator != (type const & value, inf_t){
                return value != inf_pos and value != inf_neg;
            }

            template<class type>
            friend bool operator == (inf_t, type const & value){
                return value == inf_pos or value == inf_neg;
            }

            template<class type>
            friend bool operator != (inf_t, type const & value){
                return value != inf_pos and value != inf_neg;
            }
        };

        constexpr inf_t inf {};
    }

#endif

namespace xuser::inc{
    using ::mixc::define_inf::inf;
    using ::mixc::define_inf::inf_pos;
    using ::mixc::define_inf::inf_neg;
}
