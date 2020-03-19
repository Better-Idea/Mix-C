#pragma once
#include"lang/cxx.index_of_first.hpp"
#include"define/base_type.hpp"
#include"utility/type/implicit.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline uxx index_of_first_miss(cxx<item> self, item * value, uxx count){
        cxx<item> token = { value, count };

        for (uxx i = 0; i < self.length; i++){
            if (mixc::index_of_first(token, self[i]) == not_exist){
                return i;
            }
        }
        return not_exist;
    }

    template<class item, class ... args>
    inline uxx index_of_first_miss(cxx<item> self, item value, args const & ... list){
        implicit<item> group[] = { value, list... };
        return index_of_first_miss(self, (item *)group, 1 + sizeof...(args));
    }
}
