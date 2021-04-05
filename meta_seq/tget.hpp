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
    template<uxx index_v, class first_t, class ... args_t>
    inline auto invoke(inc::tlist<first_t, args_t...>){
        if constexpr (index_v == 0){
            return (first_t *)nullptr;
        }
        else{
            return invoke<index_v - 1>(inc::tlist<args_t...>{});
        }
    }

    template<uxx index_v>
    inline auto invoke(inc::tlist<>){
        return (inc::tnull *)nullptr;
    }

    template<class tlist_t, ixx index_v>
    using tget = inc::remove_ptr<
        decltype(invoke<
            (index_v >= 0) ? index_v : tlist_t::length + uxx(index_v)
        >(tlist_t{}))
    >;
}

#endif

xexport(mixc::meta_seq_tget::tget)