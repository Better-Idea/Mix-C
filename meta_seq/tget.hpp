
#ifndef xpack_meta_seq_tget
#define xpack_meta_seq_tget
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tget::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tnull.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_ctr/cif.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tget{
    using namespace inc;

    template<class tlist, ixx index> struct meta;
    template<ixx index, class first, class ... args>
    struct meta<tlist<first, args...>, index>{
        using item = 
            cif<
                index == 0 or -index == 1 + sizeof...(args)
            >::template select<
                first
            >::template ces<
                typename meta<tlist<args...>, (index < 0 ? index : index - 1)>::item
            >;
    };

    template<ixx index>
    struct meta<tlist<>, index>{
        using item = tnull;
    };

    template<class tlist, ixx index>
    using tget = typename meta<tlist, index>::item;
}

#endif

xexport(mixc::meta_seq_tget::tget)