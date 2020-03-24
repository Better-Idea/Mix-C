#ifndef xpack_meta_seq_tpush
#define xpack_meta_seq_tpush
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tpush
        #include"define/base_type.hpp"
        #include"meta_seq/tappend.hpp"
    #pragma pop_macro("xuser")

#endif

namespace xuser::inc{
    template<class tlist, class item>
    using tpush = mixc::meta_seq_tpush::tappend<tlist, item>;
}
