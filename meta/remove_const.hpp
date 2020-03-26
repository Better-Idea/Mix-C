#ifndef xpack_meta_remove_const
#define xpack_meta_remove_const
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_remove_const
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_remove_const{
        template<class a>
        struct remove_const {
            using result = a;
        };

        template<class a>
        struct remove_const<const a> {
            using result = a;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_remove_const::remove_const;
}
