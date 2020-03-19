#pragma once
#include"memory/alloc_callback.hpp"
#include"memop/copy.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline cxx<item> clone(cxx<item> self, alloc_callback<item> alloc){
        cxx<item> r = { alloc(self.length), self.length };
        mixc::copy<item>(r, self, self.length);
        return r;
    }
}