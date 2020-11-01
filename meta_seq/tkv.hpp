#ifndef xpack_meta_seq_tkv
#define xpack_meta_seq_tkv
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tkv::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tkv{
    template<class key_type, class val_type>
    struct tkv{
        using key = key_type;
        using val = val_type;
    };
}

#endif

xexport(mixc::meta_seq_tkv::tkv)