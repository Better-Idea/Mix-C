#ifndef xpack_math_private_parallel_lut
#define xpack_math_private_parallel_lut
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_private_parallel_lut::inc
#include"define/base_type.hpp"
#include"docker/array.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_private_parallel_lut{
    template<uxx multiple_v, uxx i_v, class type_t, class ... args_t>
    inline constexpr auto parallel_lut_core(decltype(nullptr), type_t a0, args_t ... rest){
        return inc::array<type_t, 1 + sizeof...(args_t)>{ a0, rest... };
    }

    template<uxx multiple_v, uxx i_v, class type_t, class ... args_t>
    inline constexpr auto parallel_lut_core(type_t a0, args_t ... rest){
        if constexpr (i_v < multiple_v){
            return parallel_lut_core<multiple_v, i_v + 1>(a0, rest..., a0);
        }
        else{
            return parallel_lut_core<multiple_v, 0>(rest...);
        }
    }
}

namespace mixc::math_private_parallel_lut::origin{
    template<uxx multiple_v, class type_t, class ... args_t>
    inline constexpr auto parallel_lut(type_t a0, args_t ... rest){
        return parallel_lut_core<multiple_v, 0>(a0, rest..., nullptr);
    }
}

#endif

xexport_space(mixc::math_private_parallel_lut::origin)