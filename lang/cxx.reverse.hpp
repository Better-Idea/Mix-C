#pragma once
#include"memory/alloc_callback.hpp"
#include"lang/cxx.substr.hpp"

namespace mixc{
    template<class item> struct cxx;
    template<class item>
    inline cxx<item> reverse(cxx<item> self, alloc_callback<item> alloc) {
        return substr(self, -1, 0, alloc);
    }
}
