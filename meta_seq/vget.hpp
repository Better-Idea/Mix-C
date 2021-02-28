
#ifndef xpack_meta_seq_vget
#define xpack_meta_seq_vget
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_vget::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/vlist.hpp"
#include"meta_seq/vnull.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_vget{
    using namespace inc;

    template<ixx i_v>
    inline constexpr auto invoke(vlist<>){
        return vnull;
    }

    template<ixx i_v, auto first_v, auto ... values_v>
    inline constexpr auto invoke(vlist<first_v, values_v...>){
        if constexpr (i_v == 0 or -i_v == 1 + sizeof...(values_v)){
            return first_v;
        }
        else{
            return invoke<
                (i_v < 0 ? i_v : i_v - 1), values_v...
            >(vlist<values_v...>{});
        }
    }

    template<class vlist_t, ixx index_v> struct vget{
        static constexpr auto value = invoke<index_v>(vlist_t{});
    };
}

#endif

xexport(mixc::meta_seq_vget::vget)