#ifndef xpack_interface_can_alloc_seq
#define xpack_interface_can_alloc_seq
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_alloc_seq::inc
#include"define/base_type.hpp"
#include"interface/can_callback.hpp"
#include"interface/unified_seq.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_alloc_seq::origin{
    template<class seq_t>
    using ialloc_seq = inc::icallback<
        inc::unified_seq<seq_t>(uxx length)
    >;

    template<class functor_t>
    concept can_alloc_seq = inc::can_callback<
        functor_t, 
        inc::unified_seq<decltype(functor_t()(0))>(uxx)
    >;
}

#endif

xexport_space(mixc::interface_can_alloc_seq::origin)