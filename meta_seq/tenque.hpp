#ifndef xpack_meta_seq_tenque
#define xpack_meta_seq_tenque
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tenque
        #include"define/base_type.hpp"
        #include"meta_seq/tappend.hpp"
    #pragma pop_macro("xuser")

#endif

namespace xuser::inc{
    template<class tlist, class item>
    using tenque = ::mixc::meta_seq_tappend::tappend<tlist, item>;
}
