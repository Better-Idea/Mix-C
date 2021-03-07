#ifndef xpack_macro_iterator
#define xpack_macro_iterator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_iterator::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
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
    constexpr auto itr_detect_mode = 
        // 保持判定的顺序
        full_version<invoke_t, args_t...>     == true ? itr_has_loop_control | itr_has_index :
        has_loop_control<invoke_t, args_t...> == true ? itr_has_loop_control :
        has_index<invoke_t, args_t...>        == true ? itr_has_index : 
        reduced_version<invoke_t, args_t...>  == true ? itr_reduced : itr_miss_match;

    template<auto mode_v, class invoke_t, class ... args_t>
    inline loop_t itr_switch(uxx * index, invoke_t const & invoke, args_t const & ... args){
        if constexpr (mode_v == (itr_reduced)){
            invoke(args...);
            return loop_t::go_on;
        }
        if constexpr (mode_v == (itr_has_index)){
            invoke(index[0], args...);
            index[0]   += 1;
            return loop_t::go_on;
        }
        if constexpr (mode_v == (itr_has_loop_control)){
            return invoke(args...);
        }
        if constexpr (mode_v == (itr_has_loop_control | itr_has_index)){
            loop_t
            state       = invoke(index[0], args...);
            index[0]   += 1;
            return state;
        }
    }

    #define xitr_foreach(...)                                                                                          \
    template<                                                                                                          \
        class           __iterator_t,                                                                                  \
        auto            __mode = ::mixc::macro_iterator::itr_detect_mode<__iterator_t, __VA_ARGS__>                    \
    >                                                                                                                  \
    requires(__mode != ::mixc::macro_iterator::itr_miss_match)                                                         \
    void foreach(__iterator_t const & invoke) const {                                                                  \
        foreach_template<__mode, __iterator_t>(invoke);                                                                \
    }

    #define xitr_foreachx(...)                                                                                         \
    template<                                                                                                          \
        class           __iterator_t,                                                                                  \
        can_interval    __interval_t,                                                                                  \
        auto            __mode = ::mixc::macro_iterator::itr_detect_mode<__iterator_t, __VA_ARGS__>                    \
    >                                                                                                                  \
    requires(__mode != ::mixc::macro_iterator::itr_miss_match)                                                         \
    void foreach(__interval_t const & itv, __iterator_t const & invoke) const {                                        \
        foreach_template<__mode, __iterator_t>(invoke, itv);                                                           \
    }
}

namespace mixc::macro_iterator::origin{
    using mixc::macro_iterator::itr_reduced;
    using mixc::macro_iterator::itr_has_index;
    using mixc::macro_iterator::itr_has_loop_control;
    using mixc::macro_iterator::itr_miss_match;
    using mixc::macro_iterator::itr_detect_mode;
    using mixc::macro_iterator::itr_switch;
}

#endif

xexport_space(mixc::macro_iterator::origin)
