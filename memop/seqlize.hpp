#ifndef xpack_memop_seqlize
#define xpack_memop_seqlize
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_seqlize::inc
#include"memop/addressof.hpp"
#include"interface/seqptr.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_seqlize{
    // 把type转换成指定长度的无符号整数指针
    template<class type>
    inline auto seqlize(type const & value){
        #define xgen(unsigned_t)                                                \
        if constexpr (sizeof(type) % sizeof(unsigned_t) == 0){                  \
            return inc::seqptr<unsigned_t>(                                     \
                (unsigned_t *)(xref value), sizeof(type) / sizeof(unsigned_t)   \
            );                                                                  \
        } else

        xgen(u64)
        xgen(u32)
        xgen(u16)
        xgen(u08);
        #undef xgen
    }
}

#endif

xexport(mixc::memop_seqlize::seqlize)