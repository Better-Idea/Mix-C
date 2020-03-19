#pragma once
#include"define/base_type.hpp"
#include"memory/alloc_callback.hpp"
// #include"cxx.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline cxx<item> fill_bends(cxx<item> self, uxx left_count, uxx right_count, item value, alloc_callback<item> alloc){
        uxx             total_length = left_count + right_count + self.length;
        cxx<item>       r(alloc(total_length), total_length);
        mixc::fill<item>(r, value, left_count);
        mixc::copy<item>(r.backward(left_count), self, self.length);
        mixc::fill<item>(r.backward(left_count + self.length), value, right_count);
        return r;
    }

    template<class item>
    inline cxx<item> fill_bends(cxx<item> self, uxx left_count, uxx right_count, alloc_callback<item> alloc){
        return fill_bends(self, left_count, right_count, ' ', alloc);
    }
}
