#ifndef xpack_meta_seq_tmerge
#define xpack_meta_seq_tmerge
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tmerge
#include"define/base_type.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tmerge{
    using namespace inc;

    template<class list_a, class list_b> struct tmerge;
    template<class ... args_a, class ... args_b> 
    struct tmerge<tlist<args_a ...>, tlist<args_b ...>>{
        using new_list = tlist<args_a ..., args_b ...>;
    };
}

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tmerge::tmerge;
}
