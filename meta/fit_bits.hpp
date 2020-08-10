#ifndef xpack_meta_fit_bits
#define xpack_meta_fit_bits
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_fit_bits
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_fit_bits{
    template<uxx bits>
    struct meta{
        static auto invoke(){
            static_assert(bits <= 64);

            if constexpr (bits <= 8){
                return u08();
            }
            else if constexpr (bits <= 16){
                return u16();
            }
            else if constexpr (bits <= 32){
                return u32();
            }
            else{
                return u64();
            }
        }
    };

    template<uxx bits>
    using fit_bits = decltype(meta<bits>::invoke());
}

#endif

namespace xuser::inc{
    using ::mixc::meta_fit_bits::fit_bits;
}
