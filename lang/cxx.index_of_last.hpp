#pragma once
#include"define/base_type.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline cxx<item> index_of_last(cxx<item> self, item value){
        for (uxx i = self.length; i--; ){
            if (self[i] == value){
                return i;
            }
        }
        return not_exist;
    }
}
