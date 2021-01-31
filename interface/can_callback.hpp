#ifndef xpack_interface_can_callback
#define xpack_interface_can_callback
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_callback::inc
#include"macro/xinterface.hpp"
#include"memop/signature.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_callback::origin{
    template<class func> struct icallback;

    template<class ret, class ... args>
    struct icallback<ret(args...)>{
        template<class object>
        requires(inc::signature<ret(args...)>::has(& object::operator()))
        constexpr icallback(object const & this_ref){
            struct closure{
                static ret shell(object * this_ptr, args ... list){
                    return this_ptr->operator()(list...);
                }
            };
            
            this_ptr    = xref this_ref;
            this_func   = func_t(xref closure::shell);
        }

        constexpr icallback(){}

        ret operator()(args ... list){
            return this_func(this_ptr, list...);
        }

    private:
        using func_t = ret(*)(args...);
        voidp   this_ptr  = nullptr;
        func_t  this_func = nullptr;
    };

    template<class functor_t, class signature>
    concept can_callback = requires(functor_t const & call){
        icallback<signature>(call);
    };
}

#endif

xexport_space(mixc::interface_can_callback::origin)