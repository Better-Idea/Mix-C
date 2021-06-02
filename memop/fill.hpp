#ifndef xpack_memop_fill
#define xpack_memop_fill
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_fill::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_fill{
    template<bool with_operator_v, class a0_t, class a1_t>
    inline void fill_core(a0_t & target, a1_t const & source, uxx count) {
        for (uxx i = 0; i < count; i++) {
            if constexpr (with_operator_v){
                target[i] = (a1_t &)source;
            }
            else{
                inc::copy(xref(target[i]), source);
            }
        }
    }
}

namespace mixc::memop_fill::origin{
    template<class a0_t, class a1_t>
    inline void fill_with_operator(a0_t & target, a1_t const & source, uxx count) {
        fill_core<true, a0_t, a1_t>(target, source, count);
    }

    template<class a0_t, class a1_t>
    inline void fill_with_operator(a0_t && target, a1_t const & source, uxx count) {
        fill_core<true, a0_t, a1_t>((a0_t &)target, source, count);
    }

    template<class a0_t, class a1_t>
    inline void fill(a0_t & target, a1_t const & source, uxx count) {
        fill_core<false, a0_t, a1_t>(target, source, count);
    }

    template<class a0_t, class a1_t>
    inline void fill(a0_t && target, a1_t const & source, uxx count) {
        fill_core<false, a0_t, a1_t>((a0_t &)target, source, count);
    }
}

#endif

xexport_space(mixc::memop_fill::origin)