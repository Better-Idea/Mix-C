#ifndef xpack_meta_seq_tmarge
#define xpack_meta_seq_tmarge
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tmarge
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tmarge{
        using namespace inc;

        template<class list_a, class list_b> struct tmarge;
        template<class ... args_a, class ... args_b> 
        struct tmarge<tlist<args_a ...>, tlist<args_b ...>>{
            using new_list = tlist<args_a ..., args_b ...>;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tmarge::tmarge;
}
