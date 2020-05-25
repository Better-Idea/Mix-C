#ifndef xpack_instruction_index_of_first_set
#define xpack_instruction_index_of_first_set
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_index_of_first_set
        #include"configure.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_index_of_first_set{
        template<class type>
        inline uxx index_of_first_set(type v){
            uxx i;
            if constexpr (xis_x86){
                if constexpr (xis_os64){
                    i = __builtin_ctzll(v);
                }
                else if constexpr (sizeof(v) == 8){
                    i = u32(v) ? __builtin_ctz(u32(v)) : __builtin_ctz(v >> 32) + 32;
                }
                else {
                    i = __builtin_ctz(v);
                }
            }
            return v == 0 ? not_exist : i;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_index_of_first_set::index_of_first_set;
}
