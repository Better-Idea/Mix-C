#pragma once
#include"define/base_type.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline uxx index_of_first(cxx<item> self, item value){
        for (uxx i = 0; i < self.length; i++){
            if (self[i] == value){
                return i;
            }
        }
        return not_exist;
    }
}
