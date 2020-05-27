#ifndef xpack_meta_more_fit
#define xpack_meta_more_fit
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_more_fit
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
        #include"meta/has_cast.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_more_fit{
        template<
            class the_target = void,
            uxx  index_value = not_exist, 
            uxx  size_different = uxx(-1), 
            bool has_full_match = false>
        struct record{
            using target = the_target;
            static constexpr uxx  index      = index_value;
            static constexpr uxx  different  = size_different;
            static constexpr bool full_match = has_full_match;
        };

        template<uxx count, class source, class ... target> struct meta;
        template<uxx count, class source>
        struct meta<count, source>{
            static constexpr auto test(){
                return record<>();
            }
        };

        template<uxx count, class source, class target, class ... rest>
        struct meta<count, source, target, rest...> : meta<count, source, rest...>{
            static constexpr auto test(){
                constexpr auto next = meta<count, source, rest...>::test();
                constexpr auto curr_diff = 
                    sizeof(target) > sizeof(source) ? 
                    sizeof(target) - sizeof(source) : 
                    sizeof(source) - sizeof(target);
                using next_target = typename decltype(next)::target;

                if  constexpr (inc::is_same<target, source>){
                    return record<target, count - sizeof...(rest) - 1, curr_diff, true>();
                }
                else if constexpr (
                    next.full_match or 
                    next.different < curr_diff or 
                    not inc::has_cast<target, source>){
                    return next;
                }
                else{
                    return record<target, count - sizeof...(rest) - 1, curr_diff>();
                }
            }
        };

        template<class source, class target, class ... rest>
        constexpr uxx more_fit = meta<sizeof...(rest) + 1, source, target, rest...>::test().index;
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_more_fit::more_fit;
}
