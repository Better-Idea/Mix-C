#ifndef xpack_meta_seq_tselector_key
#define xpack_meta_seq_tselector_key
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tselector_key
        #include"define/base_type.hpp"
        #include"meta_seq/template/tselector.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tselector_key{
        xmake_selector(key);
    }

#endif

namespace xuser::inc{
    using mixc::meta_seq_tselector_key::tselector_key;
}
