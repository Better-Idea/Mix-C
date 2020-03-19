#pragma once
#include"meta_seq/tlist.hpp"

namespace mixc{
    struct dummy_t{
        template<class ... args>
        dummy_t(args const & ...){}
        using member_list = mixc::tlist<>;
    };
}
