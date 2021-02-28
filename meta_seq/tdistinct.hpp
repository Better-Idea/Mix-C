
#ifndef xpack_meta_seq_tdistinct
#define xpack_meta_seq_tdistinct
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tdistinct::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tdistinct{
    using namespace inc;

    template<class first_t, class ... args_t, class ... result_args_t>
    inline auto invoke(tlist<first_t, args_t...>, tlist<result_args_t...>){
        using old_list = tlist<result_args_t...>;
        using new_list = tlist<result_args_t..., first_t>;
        if constexpr (tin<tlist<result_args_t...>, first_t>){
            return invoke(
                tlist<args_t...>{},
                old_list{}
            );
        }
        else{
            return invoke(
                tlist<args_t...>{},
                new_list{}
            );
        }
    }

    template<class result_t>
    inline auto invoke(tlist<>, result_t r){
        return r;
    }

    template<class list_t>
    struct tdistinct{
        using new_list = decltype(
            invoke(list_t{}, tlist<>{})
        );
    };
}

#endif

xexport(mixc::meta_seq_tdistinct::tdistinct)