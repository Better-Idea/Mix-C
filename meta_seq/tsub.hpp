#ifndef xpack_meta_seq_tsub
#define xpack_meta_seq_tsub
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tsub::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#include"meta_seq/tmerge.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tsub{
    using namespace inc;

    template<class minus_t, class first_t, class ... args_t, class ... result_args_t>
    inline auto invoke(
        tlist<first_t, args_t...>, 
        minus_t                     minus, 
        tlist<result_args_t...>     result){

        if constexpr (tin<minus_t, first_t>){
            return invoke(tlist<args_t...>{}, minus, result);
        }
        else{
            return invoke(tlist<args_t...>{}, minus, tlist<result_args_t..., first_t>{});
        }
    }

    template<class minus_t, class result_t>
    inline auto invoke(tlist<>, minus_t, result_t){
        return result_t{};
    }

    template<class tlist0_t, class tlist1_t>
    struct tsub{
        using new_list = decltype(
            invoke(tlist0_t{}, tlist1_t{}, tlist<>{})
        );
    };
}

#endif

xexport(mixc::meta_seq_tsub::tsub)