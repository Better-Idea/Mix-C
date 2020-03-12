#pragma once
#include"meta_seq/tnull.hpp"
#include"meta_seq/tlist.hpp"

namespace mixc {
    template<class tlist> struct tdeque;
    template<class first, class ... args>
    struct tdeque<tlist<first, args...>>{
        using item = first;
        using new_list = tlist<args...>;
    };
    
    template<>
    struct tdeque<tlist<>>{
        using item = tnull;
        using new_list = tlist<>;
    };
}
