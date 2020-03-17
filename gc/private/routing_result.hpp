#pragma once
#include"define/base_type.hpp"

namespace mixc::inner_gc{
    struct routing_result{
        uxx     can_arrive_root;
        ixx     degree_dvalue;

        routing_result() : routing_result(0){}
        routing_result(uxx can_arrive_root, ixx degree_dvalue = 0) : 
            can_arrive_root(can_arrive_root), 
            degree_dvalue(degree_dvalue) {}
    };
}
