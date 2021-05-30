#ifndef xpack_instruction_index_of_first_set
#define xpack_instruction_index_of_first_set
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_index_of_first_set::inc
#include"configure/platform.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"memop/cast.hpp"
#include"meta/unsigned_type.hpp"
#include"meta/is_number.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_index_of_first_set{
    template<inc::is_number number_t>
    inline uxx index_of_first_set(number_t v){
        // 避免有符号数的符号位扩展
        using u_t   = inc::unsigned_type<number_t>;
        u_t uv      = inc::cast<u_t>(v);

        if (uv == 0) {
            return not_exist;
        }

        #if xis_msvc_native
            unsigned long i;

            #if xis_os64
                _BitScanForward64(& i, uv);
            #elif xis_os32
                if constexpr (sizeof(v) == 8) {
                    if (u32(uv) != 0) {
                        _BitScanForward(& i, u32(uv));
                    }
                    else{
                        _BitScanForward(& i, u32(uv >> 32));
                        i += 32;
                    }
                }
                else {
                    _BitScanForward(& i, u32(uv));
                }
            #else
                #error "mismatch"
            #endif

            return i;
        #else
            #if xis_os64
                return __builtin_ctzll(uv);
            #elif xis_os32
                if constexpr (sizeof(uv) == 8){
                    return u32(uv) ? __builtin_ctz(u32(uv)) : __builtin_ctz(uv >> 32) + 32;
                }
                else {
                    return __builtin_ctz(uv);
                }
            #else
                #error "mismatch"
            #endif
        #endif
    }
}

#endif

xexport(mixc::instruction_index_of_first_set::index_of_first_set)