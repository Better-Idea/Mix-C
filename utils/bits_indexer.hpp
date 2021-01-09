#ifndef xpack_utils_bits_indexer
#define xpack_utils_bits_indexer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_bits_indexer::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_bits_indexer::origin{
    template<uxx bits_per_item>
    xstruct(
        xtmpl(bits_indexer, bits_per_item),
        xprif(bits, u64)
    )
        bits_indexer(u64 value = 0):
            bits(value){
        }

        uxx insert(uxx index, uxx value){
            index  *= (bits_per_item);
            u64 msk = (u64(-1) << index);
            u64 low = (bits & ~msk);
            u64 hig = (bits &  msk) << bits_per_item;
            u64 out = (bits >> (64 - bits_per_item));
            u64 ins = (value << index);
            bits    = (low | hig | ins);
            return ins;
        }

        void remove(uxx index){
            index  *= (bits_per_item);
            u64 msk = (u64(-1) << index);
            u64 low = (bits & ~msk);
            u64 hig = (bits & (msk << bits_per_item)) >> bits_per_item;
            bits    = (low | hig);
        }

        uxx get(uxx index) const {
            index  *= bits_per_item;
            return (bits >> index) & 0xf;
        }

        void set(uxx index, uxx value){
            index  *=  (bits_per_item);
            bits   &= ~(0xf << index);
            bits   |=  (value << index);
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
