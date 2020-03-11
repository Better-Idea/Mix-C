#pragma once
#include"define/base_type.hpp"

namespace mixc{
    template<class type> struct function;

    #define gen(a,...)                                                      \
    template<a class ret_type, class ... list>                              \
    struct function<ret_type __VA_ARGS__(list...)> {                        \
        static constexpr uxx args_count = sizeof...(list);                  \
        using return_type = ret_type;                                       \
        template<ixx index>                                                 \
        struct args {                                                       \
            using result = typename flow::select<index, list...>::result;   \
        };                                                                  \
    };

    #define gen_arg class object, 
    gen(,);
    gen(gen_arg, (object::*));
    #undef gen
    #undef gen_arg
}
