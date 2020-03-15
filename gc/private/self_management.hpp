#pragma once
#include"define/base_type.hpp"

namespace mixc{
    struct self_management{
    protected:
        template<class guide> uxx routing(guide, void * root);
        template<class guide> void clear_footmark(guide);
    };
}
