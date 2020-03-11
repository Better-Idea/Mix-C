#pragma once
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#include"meta_ctr/cif.hpp"

namespace mixc {
    template<class tlist, class item> struct tdistinct_append;
    template<class item, class ... args>
    struct tdistinct_append<tlist<args...>, item>{
        using new_list = 
            typename cif<
                tin<tlist<args...>, item>,
                tlist<args...>,
                tlist<args..., item>
            >::result;
    };
}
