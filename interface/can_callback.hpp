#ifndef xpack_interface_can_callback
#define xpack_interface_can_callback
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_callback::inc
#include"macro/xinterface.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_callback::origin{
    template<class func> struct icallback;

    template<class ret, class ... args> 
    xinterface(
        xspec(icallback, ret(args...)),
        xoper(ret(args...))
    );

    template<class functor_t, class signature>
    concept can_callback = requires(functor_t const & call, icallback<signature> * ptr){
        xnew(ptr) icallback<signature>(call);
    };
}

#endif

xexport_space(mixc::interface_can_callback::origin)