
#ifndef xpack_meta_seq_tget
#define xpack_meta_seq_tget
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tget::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/remove_ptr.hpp"
#include"meta_seq/tnull.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tget{
    template<ixx index, class first, class ... args>
    inline auto invoke(inc::tlist<first, args...>){
        if constexpr (index == 0 or -index == 1 + sizeof...(args)){
            return (first *)nullptr;
        }
        else{
            return invoke<
                (index < 0 ? index : index - 1)
            >(inc::tlist<args...>{});
        }
    }

    template<ixx index>
    inline auto invoke(inc::tlist<>){
        return (inc::tnull *)nullptr;
    }

    template<class tlist, ixx index>
    using tget = inc::remove_ptr<
        decltype(invoke<index>(tlist{}))
    >;
}

#endif

xexport(mixc::meta_seq_tget::tget)