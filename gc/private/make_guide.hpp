#pragma once
#include"gc/private/routing.hpp"
#include"gc/private/collect.hpp"
#include"meta/is_class.hpp"

namespace mixc::inner_gc{
    template<class root>
    inline auto make_guide() {
        if constexpr(is_class<root>){
            using routing_result = typename inner_gc::routing<root>::result_kvlist;
            using guide = typename inner_gc::collect<root, routing_result>::result_list;
            return guide();
        }
        else{
            return tlist<>();
        }
    }
}
