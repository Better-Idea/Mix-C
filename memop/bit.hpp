#ifndef xpack_memop_bits
#define xpack_memop_bits
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_bits
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_bits{
        template<class bits_t, class bit_t>
        inline void bit(bits_t * des, bit_t const & bit, bool value){
            if (value){
                des[0] = bits_t(u64(des[0]) | u64(bit));
            }
            else{
                des[0] = bits_t(u64(des[0]) & ~u64(bit));
            }
        }

        template<class bits_t, class bit_t>
        inline bool bit(bits_t const & bits, bit_t const & bit){
            return (u64(bits) & u64(bit)) != 0;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_bits::bit;
}
