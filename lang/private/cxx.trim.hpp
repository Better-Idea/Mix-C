#pragma once
#include"memory/alloc_callback.hpp"

namespace mixc::inner_trim{
    template<class item, uxx i> 
    inline auto layout_args(item * buf, alloc_callback<item> alloc){
        return alloc;
    }

    template<class item, uxx i>
    inline auto layout_args(item * buf){
        return buf;
    }

    template<class item, uxx i = 0, class ... args> 
    inline auto layout_args(
        item *           buf, 
        item             first, 
        args const & ... list){
        buf[i] = first;
        return layout_args<item, i + 1>(buf, list...);
    }
}
