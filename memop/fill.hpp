#ifndef xpack_memop_fill
#define xpack_memop_fill
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_fill::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_fill{
    template<bool with_operator, class a, class b>
    inline void fill_core(a & target, b const & source, uxx count) {
        for (uxx i = 0; i < count; i++) {
            if constexpr (with_operator){
                target[i] = (b &)source;
            }
            else{
                inc::copy(xref target[i], source);
            }
        }
    }
}

namespace mixc::memop_fill::origin{
    template<class a, class b>
    inline void fill_with_operator(a & target, b const & source, uxx count) {
        fill_core<true, a, b>(target, source, count);
    }

    template<class a, class b>
    inline void fill_with_operator(a && target, b const & source, uxx count) {
        fill_core<true, a, b>((a &)target, source, count);
    }

    template<class a, class b>
    inline void fill(a & target, b const & source, uxx count) {
        fill_core<false, a, b>(target, source, count);
    }

    template<class a, class b>
    inline void fill(a && target, b const & source, uxx count) {
        fill_core<false, a, b>((a &)target, source, count);
    }
}

#endif

xexport_space(mixc::memop_fill::origin)