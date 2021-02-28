#ifndef xpack_meta_seq_tpop
#define xpack_meta_seq_tpop
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tpop::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tnull.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tpop{
    using namespace inc;
    // 本来这样写挺好的
    // 但目前C++不准这样做
    // error: class template partial specialization contains template parameters that cannot be deduced
    // this partial specialization will never be used

    // template<class list_t> struct tpop;
    // template<class last_t, class ... args_t>
    // struct tpop<tlist<args_t..., last_t>>{
    //     using item_t = last_t;
    //     using new_tlist = tlist<args_t...>;
    // };

    // template<>
    // struct tpop<tlist<>>{
    //     using item_t = tnull;
    //     using new_tlist = tlist<>;
    // };
    template<class first_t, class second_t, class ... args_t, class ... result_args_t>
    inline auto invoke(tlist<first_t, second_t, args_t...>, tlist<result_args_t...>){
        return invoke(
            tlist<second_t, args_t...>(),
            tlist<result_args_t..., first_t>()
        );
    }

    template<class last_t, class ... result_args_t>
    inline auto invoke(tlist<last_t>, tlist<result_args_t...>){
        struct pair{
            using item_t    = last_t;
            using new_list  = tlist<result_args_t...>;
        };
        return pair();
    }

    template<class ... result_args_t>
    inline auto invoke(tlist<>, tlist<result_args_t...>){
        struct pair{
            using item_t = tnull;
            using new_list = tlist<result_args_t...>;
        };
        return pair();
    }

    template<class list_t>
    struct tpop{
        using item_t    = typename decltype(
            invoke(list_t{}, tlist<>{})
        )::item_t;

        using new_list  = typename decltype(
            invoke(list_t{}, tlist<>{})
        )::new_list;
    };
}

#endif

xexport(mixc::meta_seq_tpop::tpop)