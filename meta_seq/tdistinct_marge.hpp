#ifndef xpack_meta_seq_tdistinct_merge
#define xpack_meta_seq_tdistinct_merge
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tdistinct_merge::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tdistinct_merge{
    using namespace inc;

    template<class first_t, class ... args_t, class ... result_args_t>
    inline auto invoke(tlist<first_t, args_t...>, tlist<result_args_t...>){
        if constexpr (tin<tlist<result_args_t...>, first_t>){
            return invoke(
                tlist<args_t...>(), 
                tlist<result_args_t...>()
            );
        }
        else{
            return invoke(
                tlist<args_t...>(), 
                tlist<result_args_t..., first_t>()
            );
        }
    }

    template<class result_list>
    inline auto invoke(tlist<>, result_list r){
        return r;
    }

    template<class tlist0_t, class tlist1_t>
    struct tdistinct_merge{
        using new_list = decltype(
            invoke(
                tlist1_t{}, 
                invoke(tlist0_t{}, tlist<>{})
            )
        );
    };
}

#endif

xexport(mixc::meta_seq_tdistinct_merge::tdistinct_merge)