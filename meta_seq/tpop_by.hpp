#ifndef xpack_meta_seq_tpop_by
#define xpack_meta_seq_tpop_by
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tpop_by::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tselector_key.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tpop_by{
    using namespace inc;

    template<
        template<class> class   selector_t, 
        class                   key_t,
        class                   first_t, 
        class ...               args_t, 
        class ...               vals_t, 
        class ...               rest_t
    >
    inline auto invoke(key_t * k, tlist<first_t, args_t...>, tlist<tlist<vals_t...> , tlist<rest_t...>>){
        using current_key = typename selector_t<first_t>::item_t;

        if constexpr (tlist<args_t...> src; is_same<current_key, key_t>){
            return invoke<selector_t>(k, src, tlist<tlist<vals_t..., first_t> , tlist<rest_t...>>{});
        }
        else{
            return invoke(k, src, tlist<tlist<vals_t...> , tlist<rest_t..., first_t>>{});
        }
    }

    template<
        template<class> class   selector_t, 
        class                   key_t, 
        class ...               vals_t, 
        class ...               rest_t
    >
    inline auto invoke(key_t *, tlist<>, tlist<tlist<vals_t...> , tlist<rest_t...>> r){
        return r;
    }

    template<class ... vals_t, class ... rest_t>
    inline auto val(tlist<tlist<vals_t...>, tlist<rest_t...>>){
        return tlist<vals_t...>();
    }

    template<class ... vals_t, class ... rest_t>
    inline auto rest(tlist<tlist<vals_t...> , tlist<rest_t...>>){
        return tlist<rest_t...>();
    }

    template<class tlist_t, class key_t, template<class> class selector_t>
    struct tpop_by{
        using item_list = decltype(
            val(
                decltype(
                    invoke<selector_t>((key_t *)nullptr, tlist_t{}, tlist<tlist<>, tlist<>>{})
                ){}
            )
        );

        using new_list = decltype(
            rest(
                decltype(
                    invoke<selector_t>((key_t *)nullptr, tlist_t{}, tlist<tlist<>, tlist<>>{})
                ){}
            )
        );
    };
}

#endif

xexport(mixc::meta_seq_tpop_by::tpop_by)