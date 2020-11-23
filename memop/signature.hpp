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

    template<class ret, class ... args>
    struct signature<ret(args...)>{ 
        template<class owner>
        static auto fetch(ret(owner::* member_func)(args...)){ 
            return member_func;
        } 

        template<class owner>
        static auto fetch(ret(owner::* member_func)(args...) const){ 
            return member_func;
        }

        template<class owner>
        static constexpr bool has(ret(owner::*)(args...)){ 
            return true;
        }

        template<class owner>
        static constexpr bool has(ret(owner::*)(args...) const){ 
            return true;
        }
    };
}

#endif

xexport(mixc::memop_signature::signature)