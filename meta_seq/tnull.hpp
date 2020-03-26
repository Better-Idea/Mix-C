#ifndef xpack_meta_seq_tnull
#define xpack_meta_seq_tnull
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tnull
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tnull{
        struct tnull{};
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tnull::tnull;
}
