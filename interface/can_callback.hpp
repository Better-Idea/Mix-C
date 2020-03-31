#ifndef xpack_interface_can_callback
#define xpack_interface_can_callback
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_can_callback
        #include"define/base_type.hpp"
        #include"macro/xitf.hpp"
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_can_callback{
        template<class func> struct can_callback;
        template<class ret, class ... args> 
        struct can_callback<ret(args...)> : mixc::macro_private_callable::callable_t {
            using signature = mixc::macro_xitf::signature<ret, args...>;
            using base::operator=;
            using base::operator==;
            using base::operator!=;

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
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_can_callback::can_callback;
    using namespace ::mixc::macro_private_callable;
}
