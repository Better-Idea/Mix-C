#pragma once
#include"meta_seq/tlist.hpp"

namespace mixc{
    template<class tlist, class item> struct tappend;
    template<class item, class ... args>
    struct tappend<tlist<args...>, item>{
        using new_list = tlist<args..., item>;
    };
}
