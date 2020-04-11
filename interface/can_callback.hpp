#ifndef xpack_interface_can_callback
#define xpack_interface_can_callback
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_can_callback
        #include"define/base_type.hpp"
        #include"macro/private/callable.hpp"
        #include"memop/signature.hpp"
        #include"memop/addressof.hpp"
        #include"meta/remove_membership.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_can_callback{
        template<class func> struct can_callback;
        template<class ret, class ... args> 
        struct can_callback<ret(args...)> : mixc::macro_private_callable::callable_t {
            using the_t     = can_callback<ret(args...)>;
            using signature = inc::signature<ret, args...>;
            using base::operator=;
            using base::operator==;

            can_callback(){}

            template<class object>
            can_callback(object const & impl){
                __object    = inc::addressof(impl);
                __func_list = (void **)signature::check(& object::operator());
            }

            ret operator()(args ... list) const {
                return signature::call(__object, (void *)__func_list, list...);
            }
        };

        template<class ret, class ... args> 
        struct can_callback<ret(*)(args...)> : 
            can_callback<ret(args...)>{
            using the_t = can_callback<ret(args...)>;
            using the_t::the_t;
        };

        struct sugar{
            template<class callback>
            auto operator * (callback const & call) const {
                using lambda = decltype(& callback::operator());
                using pure   = typename inc::remove_membership<lambda>::result;
                return typename can_callback<pure>::the_t((callback &)call);
            }
        };
    }

    #define xcb ::mixc::interface_can_callback::sugar() *

#endif

namespace xuser::inc{
    using ::mixc::interface_can_callback::can_callback;
}

#include"macro/private/callable.hpp"
