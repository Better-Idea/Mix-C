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
    template<uxx multiple, uxx i, class type, class ... args>
    inline constexpr auto parallel_lut_core(decltype(nullptr), type a0, args ... rest){
        return inc::array<type, 1 + sizeof...(args)>{ a0, rest... };
    }

    template<uxx multiple, uxx i, class type, class ... args>
    inline constexpr auto parallel_lut_core(type a0, args ... rest){
        if constexpr (i < multiple){
            return parallel_lut_core<multiple, i + 1>(a0, rest..., a0);
        }
        else{
            return parallel_lut_core<multiple, 0>(rest...);
        }
    }
}

namespace mixc::math_private_parallel_lut::origin{
    template<uxx multiple, class type, class ... args>
    inline constexpr auto parallel_lut(type a0, args ... rest){
        return parallel_lut_core<multiple, 0>(a0, rest..., nullptr);
    }
}

#endif

xexport_space(mixc::math_private_parallel_lut::origin)