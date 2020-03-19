#pragma once
#include"define/base_type.hpp"
#include"macro/xindex_rollback.hpp"
#include"memory/alloc_callback.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline cxx<item> insert(cxx<item> self, ixx index, cxx<item> value, alloc_callback<item> alloc){
        xindex_rollback(self.length, index, + 1);

        uxx         target_length = self.length + value.length;
        cxx<item>   r = { alloc(target_length), target_length };
        mixc::copy<item>(r, self, index);
        mixc::copy<item>(r.backward(index), value, value.length);
        mixc::copy<item>(r.backward(index + value.length), self.backward(index), self.length - index);
        return r;
    }
}
