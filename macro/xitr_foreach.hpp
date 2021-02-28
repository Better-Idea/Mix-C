#ifndef xpack_macro_iterator
#define xpack_macro_iterator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_iterator::inc
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_iterator{
    enum : uxx{
        itr_reduced             = 0,
        itr_has_index           = 1,
        itr_has_loop_control    = 2,
        itr_miss_match          = not_exist,
    };

    template<class invoke_t, class ... args_t>
    concept full_version = requires(invoke_t lambda, loop_t state, uxx i, args_t ... list){
        state = lambda(i, list...);
    };

    template<class invoke_t, class ... args_t>
    concept has_loop_control = requires(invoke_t lambda, loop_t state, args_t ... list){
        state = lambda(list...);
    };

    template<class invoke_t, class ... args_t>
    concept has_index = requires(invoke_t lambda, uxx i, args_t ... list){
        lambda(i, list...);
    };

    template<class invoke_t, class ... args_t>
    concept reduced_version = requires(invoke_t lambda, uxx i, args_t ... list){
        lambda(list...);
    };

    template<class invoke_t, class ... args_t>
    constexpr auto detect_mode = 
        // 保持判定的顺序
        full_version<invoke_t, args_t...>     == true ? itr_has_loop_control | itr_has_index :
        has_loop_control<invoke_t, args_t...> == true ? itr_has_loop_control :
        has_index<invoke_t, args_t...>        == true ? itr_has_index : 
        reduced_version<invoke_t, args_t...>  == true ? itr_reduced : itr_miss_match;

    #define xitr_switch(mode,index,state,invoke,...) {                                                                 \
        using namespace ::mixc::macro_iterator;                                                                        \
        if constexpr (mode == (itr_reduced)) invoke(__VA_ARGS__);                                                      \
        if constexpr (mode == (itr_has_index)) invoke(index, __VA_ARGS__), index += 1;                                 \
        if constexpr (mode == (itr_has_loop_control)) state = invoke(__VA_ARGS__);                                     \
        if constexpr (mode == (itr_has_loop_control | itr_has_index)) state = invoke(index, __VA_ARGS__), index += 1;  \
    }

    #define xitr_foreach(...)                                                                                          \
    template<                                                                                                          \
        class           iterator_t,                                                                                    \
        auto            mode = ::mixc::macro_iterator::detect_mode<iterator_t, __VA_ARGS__>                            \
    >                                                                                                                  \
    requires(mode != ::mixc::macro_iterator::itr_miss_match)                                                           \
    void foreach(iterator_t const & invoke) const {                                                                    \
        foreach_template<mode, iterator_t>(invoke);                                                                    \
    }

    #define xitr_foreachx(...)                                                                                         \
    template<                                                                                                          \
        class           iterator_t,                                                                                    \
        can_interval    interval_t,                                                                                    \
        auto            mode = ::mixc::macro_iterator::detect_mode<iterator_t, __VA_ARGS__>                            \
    >                                                                                                                  \
    requires(mode != ::mixc::macro_iterator::itr_miss_match)                                                           \
    void foreach(interval_t const & itv, iterator_t const & invoke) const {                                            \
        foreach_template<mode, iterator_t>(invoke, itv);                                                               \
    }
}

#endif
