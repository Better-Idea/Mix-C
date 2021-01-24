#ifndef xpack_instruction_index_of_first_set
#define xpack_instruction_index_of_first_set
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_index_of_first_set::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_index_of_first_set{
    template<class type>
    inline uxx index_of_first_set(type v){
        uxx i;
        #if xis_x86
            #if xis_os64
                i = __builtin_ctzll(v);
            #else
                if constexpr (sizeof(v) == 8){
                    i = u32(v) ? __builtin_ctz(u32(v)) : __builtin_ctz(v >> 32) + 32;
                }
                else {
                    i = __builtin_ctz(v);
                }
            #endif
        #endif
        return v == 0 ? not_exist : i;
    }
}

#endif

xexport(mixc::instruction_index_of_first_set::index_of_first_set)