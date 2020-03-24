#ifndef xpack_meta_seq_tselector_val
#define xpack_meta_seq_tselector_val
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tselector_val
        #include"define/base_type.hpp"
        #include"meta_seq/template/tselector.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tselector_val{
        xmake_selector(val);
    }

#endif

namespace xuser::inc{
    using mixc::meta_seq_tselector_val::tselector_val;
}
