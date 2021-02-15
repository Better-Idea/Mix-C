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

    #include"meta/private/xgen.function.hpp"

    #define xa_has_object
    #include"meta/private/xgen.function.hpp"

    #define xa_has_object
    #define xa_has_const
    #include"meta/private/xgen.function.hpp"
}

#endif

xexport(mixc::meta_function::function)