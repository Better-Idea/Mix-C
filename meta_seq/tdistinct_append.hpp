#ifndef xpack_meta_seq_tdistinct_append
#define xpack_meta_seq_tdistinct_append
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tdistinct_append::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tdistinct_append{
    using namespace inc;

    // 为了兼容 msvc，这里使用带参数的方式
    template<class item_t, class ... args_t>
    inline auto invoke(item_t *, args_t * ...){
        if constexpr (tin<tlist<args_t...>, item_t>){
            return tlist<args_t...>{};
        }
        else{
            return tlist<args_t..., item_t>{};
        }
    }

    template<class tlist_t, class item_t> struct tdistinct_append;
    template<class item_t, class ... args_t>
    struct tdistinct_append<tlist<args_t...>, item_t>{
        using new_list = decltype(
            invoke((item_t *)nullptr, (args_t *)nullptr...)
        );
    };
}

#endif

xexport(mixc::meta_seq_tdistinct_append::tdistinct_append)
