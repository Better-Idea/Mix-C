#pragma once
#include"memop/base_seqlize.hpp"

namespace mixc{
    template<class a>
    inline ixx cmp(a const & left, a const & right){
        if constexpr(__is_class(a) or __is_union(a) or __is_enum(a)){
            auto l = mixc::base_seqlize(left);
            auto r = mixc::base_seqlize(right);
            for (auto i = 0; i < l.lenght; i++){
                ixx s = cmp(l.ptr[i], r.ptr[i]);
                if (s){
                    return s;
                }
            }
            return 0;
        }
        else{
            return
                left > right ? 1 : 
                left < right ? -1 : 0;
        }
    }
}
