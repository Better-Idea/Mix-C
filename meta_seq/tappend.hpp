#ifndef xpack_meta_seq_tappend
#define xpack_meta_seq_tappend
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tappend
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tappend{
        using namespace inc;

        template<class tlist, class item> struct tappend;
        template<class item, class ... args>
        struct tappend<tlist<args...>, item>{
            using new_list = tlist<args..., item>;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tappend::tappend;
}
