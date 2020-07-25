#ifndef xpack_interface_can_callback
#define xpack_interface_can_callback
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_callback
#include"define/base_type.hpp"
#include"macro/private/callable.hpp"
#include"macro/xstruct.hpp"
#include"memop/signature.hpp"
#include"memop/addressof.hpp"
#include"meta/is_same.hpp"
#include"meta/remove_membership.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_callback{
    template<class func> struct can_callback;
    template<class ret, class ... args> 
    xstruct(
        xspec(can_callback, ret(args...)),
        xpubb(inc::callable_t)
    )
        using base_t = inc::callable_t;
        using signature = inc::signature<ret(args...)>;
        using base_t::operator=;
        using base_t::operator==;

        can_callback(){}
        can_callback(decltype(nullptr)){}

        // 在 requires 中使用 is_same 代替 more_fit 达到区分 lambda 的目的
        template<class object> requires ( 
            inc::is_same<
                decltype(& object::operator()),
                ret(object::*)(args...) const
            > or 
            inc::is_same<
                decltype(& object::operator()),
                ret(object::*)(args...)
            > 
        )
        can_callback(object const & impl){
            __object    = inc::addressof(impl);
            __func_list = (void **)signature::check(& object::operator());
        }

        ret operator()(args ... list) const {
            return signature::call(__object, (void *)__func_list, list...);
        }
    $

    template<class ret, class ... args> 
    struct can_callback<ret(*)(args...)> : 
        can_callback<ret(args...)>{
        using the_t = can_callback<ret(args...)>;
        using the_t::the_t;
    };
}

#endif

namespace xuser::inc{
    using ::mixc::interface_can_callback::can_callback;
}
