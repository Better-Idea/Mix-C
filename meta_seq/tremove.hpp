#ifndef xpack_meta_seq_tremove
#define xpack_meta_seq_tremove
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tremove::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tremove{
    using namespace inc;

    template<class item_t, class first_t, class ... args_t, class ... result_args>
    inline auto invoke(item_t *, tlist<first_t, args_t...>, tlist<result_args...>){
        if constexpr (is_same<first_t, item_t>){
            return invoke(tlist<args_t...>{}, tlist<result_args...>{});
        }
        else{
            return invoke(tlist<args_t...>{}, tlist<result_args..., first_t>{});
        }
    }

    template<class item_t, class result>
    inline auto invoke(item_t *, tlist<>, result r){
        return r;
    }

    template<class tlist_t, class item_t>
    struct tremove{
        using new_list = decltype(
            invoke((item_t *)nullptr, tlist_t{}, tlist<>{})
        );
    };
}

#endif

xexport(mixc::meta_seq_tremove::tremove)