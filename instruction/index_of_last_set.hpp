#ifndef xpack_instruction_index_of_last_set
#define xpack_instruction_index_of_last_set
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_index_of_last_set::inc
#include"configure/platform.hpp"
#include"memop/cast.hpp"
#include"meta/unsigned_type.hpp"
#include"meta/is_number.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_index_of_last_set{
    template<inc::is_number number_t>
    inline uxx index_of_last_set(number_t v){
        // 避免有符号数的符号位扩展
        using u_t   = inc::unsigned_type<number_t>;
        u_t uv      = inc::cast<u_t>(v);

        if (uv == 0) {
            return not_exist;
        }

        #if xis_msvc_native
            unsigned long i;

            #if xis_os64
                _BitScanReverse64(& i, uv);
            #elif xis_os32
                if constexpr (sizeof(uv) == 8) {
                    if (u32(uv >> 32) != 0) {
                        _BitScanReverse(& i, u32(uv >> 32));
                        i += 32;
                    }
                    else{
                        _BitScanReverse(& i, u32(uv));
                    }
                }
                else {
                    _BitScanReverse(& i, u32(uv));
                }
            #else
                #error "mismatch"
            #endif

            return i;
        #else
            #if xis_os64
                return 63 - __builtin_clzll(uv);
            #else
                if constexpr (sizeof(uv) == 8){
                    return u64(uv) >> 32 ? 
                        31 - __builtin_clz(u64(uv) >> 32) + 32:
                        31 - __builtin_clz(u32(uv));
                }
                else {
                    return 31 - __builtin_clz(uv);
                }
            #endif
        #endif
    }
}

#endif

xexport(mixc::instruction_index_of_last_set::index_of_last_set)