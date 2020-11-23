#ifndef xpack_interface_iterator
#define xpack_interface_iterator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_iterator::inc
#include"interface/can_callback.hpp"
#include"macro/xexport.hpp"
#include"meta/has_cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_iterator{
    enum : uxx{
        itr_reduced             = 0,
        itr_has_index           = 1,
        itr_has_loop_control    = 2,
        itr_miss_match          = not_exist,
    };

    template<class a0, class ... args>
    using iterator_reduced = inc::icallback<void(a0, args...)>;
    
    template<class a0, class ... args>
    using iterator = inc::icallback<void(uxx index, a0, args...)>;

    template<class a0, class ... args>
    using iteratorx_reduced = inc::icallback<loop_t(a0, args...)>;

    template<class a0, class ... args>
    using iteratorx = inc::icallback<loop_t(uxx index, a0, args...)>;
    
    template<class invoke_t, class a0, class ... args>
    constexpr auto detect_mode = 
        inc::has_cast<iterator_reduced<a0, args...>, invoke_t> ? itr_reduced :
        inc::has_cast<iterator<a0, args...>, invoke_t> ? itr_has_index :
        inc::has_cast<iteratorx_reduced<a0, args...>, invoke_t> ? itr_has_loop_control :
        inc::has_cast<iteratorx<a0, args...>, invoke_t> ? itr_has_loop_control | itr_has_index : itr_miss_match;


    #define xitr_switch(mode,index,state,invoke,...) {                                                                 \
        using namespace ::mixc::interface_iterator;                                                                    \
        if constexpr (mode == (itr_reduced)) invoke(__VA_ARGS__);                                                      \
        if constexpr (mode == (itr_has_index)) invoke(index, __VA_ARGS__), index += 1;                                 \
        if constexpr (mode == (itr_has_loop_control)) state = invoke(__VA_ARGS__);                                     \
        if constexpr (mode == (itr_has_loop_control | itr_has_index)) state = invoke(index, __VA_ARGS__), index += 1;  \
    }

    #define __xitr_foreach__(template_name,modify,...)                                                                 \
    template<                                                                                                          \
        class           iterator_t,                                                                                    \
        auto            mode = ::mixc::interface_iterator::detect_mode<iterator_t, __VA_ARGS__>                        \
    >                                                                                                                  \
    requires(mode != ::mixc::interface_iterator::itr_miss_match)                                                       \
    void foreach(iterator_t const & invoke) modify {                                                                   \
        template_name<mode, iterator_t>(invoke);                                                                       \
    }

    #define __xitr_foreachx__(template_name,modify,...)                                                                \
    template<                                                                                                          \
        class           iterator_t,                                                                                    \
        can_interval    interval_t,                                                                                    \
        auto            mode = ::mixc::interface_iterator::detect_mode<iterator_t, __VA_ARGS__>                        \
    >                                                                                                                  \
    requires(mode != ::mixc::interface_iterator::itr_miss_match)                                                       \
    void foreach(interval_t const & itv, iterator_t const & invoke) modify {                                           \
        template_name<mode, iterator_t>(invoke, itv);                                                                  \
    }

    #define xitr_foreach(...)               __xitr_foreach__(foreach_template, , __VA_ARGS__)
    #define xitr_foreach_const(...)         __xitr_foreach__(foreach_template, const, __VA_ARGS__)
    
    #define xitr_foreachx(...)              __xitr_foreachx__(foreach_template, , __VA_ARGS__)
    #define xitr_foreachx_const(...)        __xitr_foreachx__(foreach_template, const, __VA_ARGS__)
}

#endif
