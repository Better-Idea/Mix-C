#ifndef xpack_meta_more_fit
#define xpack_meta_more_fit
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_more_fit
    #include"define/base_type.hpp"
    #include"meta/has_cast.hpp"
    #include"meta/has_operator_cast.hpp"
    #include"meta/is_based_on.hpp"
    #include"meta/is_same.hpp"
    #include"meta/remove_ref.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_more_fit{
        template<
            class the_target            = void,
            uxx   index_value           = not_exist,
            uxx   size_different        = uxx(-1),
            bool  full_match_v          = false,
            bool  has_operator_cast_v   = false,
            bool  has_implicit_cast_v   = false
        >
        struct record{
            using type                              = the_target;
            static constexpr uxx  index             = index_value;
            static constexpr uxx  different         = size_different;
            static constexpr bool full_match        = full_match_v;
            static constexpr bool has_operator_cast = has_operator_cast_v;
            static constexpr bool has_implicit_cast = has_implicit_cast_v;
        };

        template<uxx count, class source, class ... target> struct meta;
        template<uxx count, class source>
        struct meta<count, source>{
            static auto invoke(){
                return record<>();
            }
        };

        template<uxx count, class source, class first, class ... target>
        struct meta<count, source, first, target...> {
            static auto invoke(){
                constexpr auto diff = sizeof(first) - sizeof(source);

                using next = decltype(
                    meta<count, source, target...>::invoke()
                );

                if constexpr (next::full_match){
                    return next();
                }
                else if constexpr (inc::is_same<first, source>){
                    return record<first, count - sizeof...(target) - 1, 0, true>();
                }
                else if constexpr (next::has_operator_cast){
                    return next();
                }
                else if constexpr (
                    inc::has_operator_cast<first, source> or 
                    inc::has_operator_cast<inc::remove_ref<first> &, source> or
                    inc::has_operator_cast<inc::remove_ref<first> const &, source> or
                    inc::is_based_on<first, source>){
                    return record<first, count - sizeof...(target) - 1, 0, false, true>();
                }
                else if constexpr (inc::has_cast<first, source>){
                    if constexpr
                       (next::has_implicit_cast and 
                       (next::different >= 0 and (diff <= 0 or diff >= next::different) or
                       (next::different < 0 and (diff <= next::different)))
                    ){
                        return next();
                    }
                    else{
                        return record<first, count - sizeof...(target) - 1, diff, false, false, true>();
                    }
                }
                else{
                    return next();
                }
            }
        };

        template<class source, class target, class ... rest>
        using more_fit = decltype(
            meta<sizeof...(rest) + 1, source, target, rest...>::invoke()
        );
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_more_fit::more_fit;
}
