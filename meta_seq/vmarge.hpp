#ifndef xpack_meta_seq_vmarge
#define xpack_meta_seq_vmarge
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_vmarge
        #include"define/base_type.hpp"
        #include"meta_seq/vlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_vmarge{
        using namespace inc;

        template<class list_a, class list_b> struct vmarge;
        template<auto ... values_a, auto ... values_b> 
        struct vmarge<vlist<values_a ...>, vlist<values_b ...>>{
            using new_list = vlist<values_a ..., values_b ...>;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_vmarge::vmarge;
}
