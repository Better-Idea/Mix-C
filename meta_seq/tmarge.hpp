#pragma once
#include"meta_seq/tlist.hpp"

namespace mixc {
    template<class list_a, class list_b> struct tmarge;
    template<class ... args_a, class ... args_b> 
    struct tmarge<tlist<args_a ...>, tlist<args_b ...>>{
        using new_list = tlist<args_a ..., args_b ...>;
    };
}
