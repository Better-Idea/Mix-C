#ifndef xpack_instruction_index_of_last_set
#define xpack_instruction_index_of_last_set
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_index_of_last_set
        #include"configure.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_index_of_last_set{
        template<class type>
        inline uxx index_of_last_set(type v){
            uxx i;
            if constexpr (xis_x86){
                if constexpr (xis_os64){
                    i = 63 - __builtin_clzll(v);
                }
                else if constexpr (sizeof(v) == 8){
                    i = u64(v) >> 32 ? 
                        31 - __builtin_clz(u64(v) >> 32) + 32:
                        31 - __builtin_clz(u32(v));
                }
                else {
                    i = 31 - __builtin_clz(v);
                }
            }
            return v == 0 ? not_exist : i;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_index_of_last_set::index_of_last_set;
}
