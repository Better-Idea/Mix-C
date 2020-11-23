#ifndef xpack_memop_signature
#define xpack_memop_signature
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_signature::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_signature{
    template<class function> struct signature;

    #define xgen(...)                                                               \
    template<class ret, class ... args>                                             \
    struct signature<ret(args...) __VA_ARGS__>{                                     \
        template<class owner>                                                       \
        static auto fetch(ret(owner::* member_func)(args...) __VA_ARGS__){          \
            return member_func;                                                     \
        }                                                                           \
    };

    xgen()
    xgen(const)
    #undef xgen
}

#endif

xexport(mixc::memop_signature::signature)