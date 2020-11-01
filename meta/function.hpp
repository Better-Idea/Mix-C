#ifndef xpack_meta_function
#define xpack_meta_function
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_function::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tget.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_function{
    template<class type> struct function;

    #define gen(a,...)                                                      \
    template<a class ret_type, class ... list>                              \
    struct function<ret_type __VA_ARGS__(list...)> {                        \
        static constexpr uxx args_count = sizeof...(list);                  \
        using return_type = ret_type;                                       \
        template<ixx index>                                                 \
        using args = inc::tget<inc::tlist<list...>, index>;                 \
    };

    #define gen_arg class object, 
    gen(,);
    gen(gen_arg, (object::*));
    #undef gen
    #undef gen_arg
}

#endif

xexport(mixc::meta_function::function)