#ifndef xpack_memop_signature
#define xpack_memop_signature
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_signature::inc
#include"configure.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_signature{
    template<class function_t> struct signature;

    template<class ret_t, class ... args_t>
    struct signature<ret_t(args_t...)>{ 
        template<class owner_t>
        static auto fetch(ret_t(owner_t::* member_func)(args_t...)){ 
            return member_func;
        } 

        template<class owner_t>
        static auto fetch(ret_t(owner_t::* member_func)(args_t...) const){ 
            return member_func;
        }

        template<class owner_t>
        static constexpr bool has(ret_t(owner_t::*)(args_t...)){ 
            return true;
        }

        template<class owner_t>
        static constexpr bool has(ret_t(owner_t::*)(args_t...) const){ 
            return true;
        }
    };
}

#endif

xexport(mixc::memop_signature::signature)