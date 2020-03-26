#ifndef xpack_meta_remove_membership
#define xpack_meta_remove_membership
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_remove_membership
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_remove_membership{
        template<class object> struct remove_membership{
            using result = object;
        };

        template<class type, class object>
        struct remove_membership<type object::*>{
            using result = type;
        };

        template<class ret_t, class object, class ... args>
        struct remove_membership<ret_t (object::*)(args...) const>{
            using result = ret_t (*)(args...);
        };

        template<class ret_t, class object, class ... args>
        struct remove_membership<ret_t (object::* const)(args...)>{
            using result = ret_t (* const)(args...);
        };

        template<class ret_t, class ... args>
        struct remove_membership<ret_t (*)(args...)>{
            using result = ret_t (*)(args...);
        };

        template<class ret_t, class ... args>
        struct remove_membership<ret_t (* const)(args...)>{
            using result = ret_t (* const)(args...);
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_remove_membership::remove_membership;
}
