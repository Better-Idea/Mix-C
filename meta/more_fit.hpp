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
        class the_target            = void,
        uxx   index_value           = not_exist,
        ixx   size_different        = min_value_of<ixx>,
        bool  full_match_v          = false,
        bool  has_operator_cast_v   = false,
        bool  has_implicit_cast_v   = false,
        bool  is_narrow_or_widen_v  = false
    >
    struct record{
        using type                                  = the_target;
        static constexpr uxx  index                 = index_value;
        static constexpr ixx  different             = size_different;
        static constexpr u08  full_match            = full_match_v;
        static constexpr u08  has_operator_cast     = has_operator_cast_v;
        static constexpr u08  has_implicit_cast     = has_implicit_cast_v;
        static constexpr u08  is_narrow_or_widen    = is_narrow_or_widen_v;
    };

    template<uxx index, class source, class ... target> struct meta;
    template<uxx index, class source>
    struct meta<index, source>{
        static auto invoke(){
            return record<>();
        }
    };

    template<uxx index, class source, class first, class ... target>
    struct meta<index, source, first, target...> {
        static auto invoke(){
            using next = decltype(
                meta<index + 1, source, target...>::invoke()
            );

            xwarning_disable(6287)
            constexpr u08 is_narrow_or_widen    = 
                (inc::is_signed<source>   and inc::is_signed<first>) or
                (inc::is_unsigned<source> and inc::is_unsigned<first>);
            xwarning_enable()

            constexpr ixx  diff                 = ixx(sizeof(first) - sizeof(source));
            constexpr bool cond0 = (next::has_implicit_cast);
            constexpr bool cond1 = (next::different >= 0 and (diff < 0 or diff > next::different));
            constexpr bool cond2 = (next::different <  0 and  diff < next::different);
            constexpr bool cond3 = (next::different == diff and next::is_narrow_or_widen > is_narrow_or_widen);
            constexpr bool cond4 = (cond1 or cond2 or cond3);

            if constexpr (is_same<first, source>){
                return record<first, index, 0, true>();
            }
            else if constexpr (next::full_match or next::has_operator_cast){
                return next();
            }
            else if constexpr (
                has_operator_cast<first, source> or 
                has_operator_cast<remove_ref<first> &, source> or
                has_operator_cast<remove_ref<first> const &, source> or
                is_based_on<first, source>){
                return record<first, index, 0, false, true>();
            }
            else if constexpr (not has_cast<first, source> or (cond0 and cond4)) {
                return next();
            }
            else{
                return record<first, index, diff, false, false, true, is_narrow_or_widen>();
            }
        }
    };

    template<class source, class target, class ... rest>
    using more_fit = decltype(
        meta<0, source, target, rest...>::invoke()
    );
}

#endif

xexport(mixc::meta_more_fit::more_fit)