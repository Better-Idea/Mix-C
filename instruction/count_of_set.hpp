#ifndef xpack_instruction_count_of_set
#define xpack_instruction_count_of_set
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_count_of_set
        #include"configure.hpp"
        #include"define/base_type.hpp"
        #include<x86intrin.h>
    #pragma pop_macro("xuser")

    namespace mixc::instruction_count_of_set{
        template<class type>
        inline uxx count_of_set(type v){
            if constexpr (xis_x86){
                if constexpr (xis_os64){
                    return __builtin_popcountll(v);
                }
                else if constexpr (sizeof(v) == 8){
                    return __builtin_popcount(v & u32(-1)) + __builtin_popcount(u64(v) >> 32);
                }
                else {
                    return __builtin_popcount(v);
                }
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_count_of_set::count_of_set;
}
