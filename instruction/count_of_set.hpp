#ifndef xpack_instruction_count_of_set
#define xpack_instruction_count_of_set
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_count_of_set::inc
#include"configure.hpp"
#include"memop/cast.hpp"
#include"meta/unsigned_type.hpp"
#include"meta/is_number.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_count_of_set{
    template<inc::is_number number_t>
    inline uxx count_of_set(number_t v){
        // 避免有符号数的符号位扩展
        using u_t   = inc::unsigned_type<number_t>;
        u_t uv      = inc::cast<u_t>(v);

        #if xis_msvc_native
            #if xis_os64
                return __popcnt64(uv);
            #elif xis_os32
                if constexpr (sizeof(uv) == 8){
                    return uxx(__popcnt(uv & u32(-1)) + __popcnt(uv >> 32));
                }
                else {
                    return __popcnt(uv);
                }
            #endif
        #else
            #if xis_os64
                return __builtin_popcountll(uv);
            #elif xis_os32
                if constexpr (sizeof(uv) == 8){
                    return __builtin_popcount(uv & u32(-1)) + __builtin_popcount(u64(uv) >> 32);
                }
                else {
                    return __builtin_popcount(uv);
                }
            #endif
        #endif

    }
}

#endif

xexport(mixc::instruction_count_of_set::count_of_set)