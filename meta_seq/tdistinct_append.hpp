#ifndef xpack_meta_seq_tdistinct_append
#define xpack_meta_seq_tdistinct_append
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tdistinct_append
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#include"meta_ctr/cif.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tdistinct_append{
    using namespace inc;

    template<class tlist, class item> struct tdistinct_append;
    template<class item, class ... args>
    struct tdistinct_append<tlist<args...>, item>{
        using new_list = 
            cif< 
                tin< tlist<args...>, item > 
            >::template select<
                tlist<args...>
            >::template ces<
                tlist<args..., item>
            >;
    };
}

#endif

xexport(mixc::meta_seq_tdistinct_append::tdistinct_append)
