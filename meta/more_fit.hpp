#ifndef xpack_meta_more_fit
#define xpack_meta_more_fit
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_more_fit::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xwarning.hpp"
#include"math/const.hpp"
#include"meta/has_cast.hpp"
#include"meta/has_operator_cast.hpp"
#include"meta/is_based_on.hpp"
#include"meta/is_same.hpp"
#include"meta/is_signed.hpp"
#include"meta/is_unsigned.hpp"
#include"meta/remove_ref.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_more_fit{
    using namespace inc;
    template<
        class target_t              = void,
        uxx   index_v               = not_exist,
        ixx   different_v           = min_value_of<ixx>,
        bool  full_match_v          = false,
        bool  has_operator_cast_v   = false,
        bool  has_implicit_cast_v   = false,
        bool  is_narrow_or_widen_v  = false
    >
    struct record{
        using type                                  = target_t;
        static constexpr uxx  index                 = index_v;
        static constexpr ixx  different             = different_v;
        static constexpr u08  full_match            = full_match_v;
        static constexpr u08  has_operator_cast     = has_operator_cast_v;
        static constexpr u08  has_implicit_cast     = has_implicit_cast_v;
        static constexpr u08  is_narrow_or_widen    = is_narrow_or_widen_v;
    };

    template<uxx index_v, class source_t, class ... target_t> struct meta;
    template<uxx index_v, class source_t>
    struct meta<index_v, source_t>{
        static auto invoke(){
            return record<>();
        }
    };

    template<uxx index_v, class source_t, class first_t, class ... target_t>
    struct meta<index_v, source_t, first_t, target_t...> {
        static auto invoke(){
            using next = decltype(
                meta<index_v + 1, source_t, target_t...>::invoke()
            );

            xwarning_push()
            xwarning_disable(6287)
            constexpr u08 is_narrow_or_widen    = 
                (inc::is_signed<source_t>   and inc::is_signed<first_t>) or
                (inc::is_unsigned<source_t> and inc::is_unsigned<first_t>);
            xwarning_pop()

            constexpr ixx  diff  = ixx(sizeof(first_t) - sizeof(source_t));
            constexpr bool cond0 = (next::has_implicit_cast);
            constexpr bool cond1 = (next::different >= 0 and (diff < 0 or diff > next::different));
            constexpr bool cond2 = (next::different <  0 and  diff < next::different);
            constexpr bool cond3 = (next::different == diff and next::is_narrow_or_widen > is_narrow_or_widen);
            constexpr bool cond4 = (cond1 or cond2 or cond3);

            if constexpr (is_same<first_t, source_t>){
                return record<first_t, index_v, 0, true>{};
            }
            else if constexpr (next::full_match or next::has_operator_cast){
                return next{};
            }
            else if constexpr (
                has_operator_cast<first_t, source_t> or 
                has_operator_cast<remove_ref<first_t> &, source_t> or
                has_operator_cast<remove_ref<first_t> const &, source_t> or
                is_based_on<first_t, source_t>){
                return record<first_t, index_v, 0, false, true>{};
            }
            else if constexpr (not has_cast<first_t, source_t> or (cond0 and cond4)) {
                return next{};
            }
            else{
                return record<first_t, index_v, diff, false, false, true, is_narrow_or_widen>{};
            }
        }
    };

    template<class source_t, class target_t, class ... rest>
    using more_fit = decltype(
        meta<0, source_t, target_t, rest...>::invoke()
    );
}

#endif

xexport(mixc::meta_more_fit::more_fit)