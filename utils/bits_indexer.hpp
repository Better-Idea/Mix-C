#ifndef xpack_utils_bits_indexer
#define xpack_utils_bits_indexer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_bits_indexer::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_bits_indexer::origin{
    template<uxx bits_per_item_v>
    xstruct(
        xtmpl(bits_indexer, bits_per_item_v),
        xprif(bits, u64)
    )
        constexpr bits_indexer(u64 value = 0x0123456789abcdefULL):
            bits(value){
        }

        uxx insert(uxx index, uxx value){
            index  *= (bits_per_item_v);
            u64 msk = (u64(-1) << index);
            u64 low = (bits & ~msk);
            u64 hig = (bits &  msk) << bits_per_item_v;
            u64 out = (bits >> (64 - bits_per_item_v));
            u64 ins = (value << index);
            bits    = (low | hig | ins);
            return ins;
        }

        void remove(uxx index){
            index  *= (bits_per_item_v);
            u64 msk = (u64(-1) << index);
            u64 low = (bits & ~msk);
            u64 hig = (bits & (msk << bits_per_item_v)) >> bits_per_item_v;
            bits    = (low | hig);
        }

        uxx get(uxx index) const {
            index  *= bits_per_item_v;
            return (bits >> index) & 0xf;
        }

        void set(uxx index, uxx value){
            index  *=  (bits_per_item_v);
            bits   &= ~(u64(0xf) << index);
            bits   |=  (u64(value) << index);
        }

        void clear(){
            bits    = 0;
        }

        operator u64 () const {
            return bits;
        }

        the_t & operator= (u64 value){
            bits = value;
            return the;
        }
    $
}

#endif

xexport_space(mixc::utils_bits_indexer::origin)
