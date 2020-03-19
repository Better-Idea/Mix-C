#pragma once
#include"define/base_type.hpp"
#include"macro/xitf.hpp"

namespace mixc{
    template<class item>
    xitf(alloc_callback, <item>)
        xitf_item(operator(), item *,
            xnt(count, uxx)
        );
    xitf_end();
}
