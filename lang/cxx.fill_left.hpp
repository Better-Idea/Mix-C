#pragma once
#include"define/base_type.hpp"
#include"lang/cxx.hpp"
#include"memop/fill.hpp"
#include"memory/alloc_callback.hpp"

namespace mixc{
    template<class item>
    inline cxx<item> fill_left(cxx<item> self, uxx count, item value, alloc_callback<item> alloc){
        uxx       length = self.length + count;
        cxx<item> r = { alloc(length), length };
        mixc::fill<item>(r, value, count);
        mixc::copy<item>(r.backward(count), self, self.length);
        return r;
    }

    template<class item>
    inline cxx<item> fill_left(cxx<item> self, uxx count, alloc_callback<item> alloc){
        return fill_left(self, count, ' ', alloc);
    }
}
