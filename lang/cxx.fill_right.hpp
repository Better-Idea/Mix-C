#pragma once
#include"define/base_type.hpp"
#include"lang/cxx.hpp"
#include"memop/fill.hpp"
#include"memory/alloc_callback.hpp"

namespace mixc{
    template<class item>
    inline cxx<item> fill_right(cxx<item> self, uxx count, item value, alloc_callback<item> alloc){
        uxx       length = self.length + count;
        cxx<item> r = { alloc(length), length };
        mixc::copy<item>(r, self, self.length);
        mixc::fill<item>(r.backward(self.length), value, count);
        return r;
    }

    template<class item>
    inline cxx<item> fill_right(cxx<item> self, uxx count, alloc_callback<item> alloc){
        return fill_right(self, count, ' ', alloc);
    }
}
