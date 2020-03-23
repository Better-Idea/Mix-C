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
            static constexpr bool is_normal_object   = true;
            static constexpr bool is_member_object   = false;
            static constexpr bool is_normal_function = false;
            static constexpr bool is_member_function = false;
        };

        template<class type, class object>
        struct remove_membership<type object::*>{
            using result = type;
            static constexpr bool is_normal_object   = false;
            static constexpr bool is_member_object   = true;
            static constexpr bool is_normal_function = false;
            static constexpr bool is_member_function = false;
        };

        template<class ret_t, class object, class ... args>
        struct remove_membership<ret_t (object::*)(args...)>{
            using result = ret_t (*)(args...);
            static constexpr bool is_normal_object   = false;
            static constexpr bool is_member_object   = false;
            static constexpr bool is_normal_function = false;
            static constexpr bool is_member_function = true;
        };

        template<class ret_t, class ... args>
        struct remove_membership<ret_t (*)(args...)>{
            using result = ret_t (*)(args...);
            static constexpr bool is_normal_object   = false;
            static constexpr bool is_member_object   = false;
            static constexpr bool is_normal_function = true;
            static constexpr bool is_member_function = false;
        };
    }

#endif

namespace xuser::inc{
    using namespace mixc::meta_remove_membership;
}
