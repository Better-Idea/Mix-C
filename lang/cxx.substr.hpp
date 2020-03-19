#pragma once
#include"define/base_type.hpp"
#include"macro/xindex_rollback.hpp"
#include"memory/alloc_callback.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline cxx<item> substr(cxx<item> self, ixx start, ixx end, alloc_callback<item> alloc){
        if (self.length == 0) {
            return cxx<item>();
        }

        xindex_rollback(self.length, start);
        xindex_rollback(self.length, end);

        uxx    target_length;
        item * buf;
        item * temp;

        if (start <= end) {
            target_length   = uxx(end - start + 1);
            temp            = buf = alloc(target_length);
            while(start <= end){
                temp[0]     = self[start];
                temp       += 1;
                start      += 1;
            }
        }
        else{
            target_length   = uxx(start - end + 1);
            temp            = buf = alloc(target_length);
            while(start >= end){
                temp[0]     = self[start];
                temp       += 1;
                start      -= 1;
            }
        }
        return cxx<item>(buf, target_length);
    }
}
