#ifndef xpack_algo_shuffle
#define xpack_algo_shuffle
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_shuffle::inc
#include"interface/unified_seq.hpp"
#include"macro/xexport.hpp"
#include"math/random.hpp"
#include"memop/addressof.hpp"
#include"memop/swap.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_shuffle{
    template<inc::can_unified_seqlize seq_t>
    inline void shuffle_core(seq_t seq){
        uxx length  = seq.length();
        uxx i       = 0;
        uxx w       = uxx(1) << (sizeof(uxx) * 8 / 2);
        uxx a;
        uxx b;
        uxx r;

        if (w > length) for(; i < length; i++){         // 复用生成的随机数
            r  = inc::random<uxx>();
            a  = r >> (sizeof(uxx) * 4);                // 使用随机数的高位
            b  = r & (uxx(-1) >> (sizeof(uxx) * 4));    // 使用随机数的低位
            a %= length;
            b %= length;

            inc::swap(
                xref seq[a],
                xref seq[b]
            );
        }
        else for(; i < length; i++){
            a = inc::random<uxx>() % length;
            b = inc::random<uxx>() % length;

            inc::swap(
                xref seq[a],
                xref seq[b]
            );
        }
    }

    template<inc::can_unified_seqlize seq_t>
    inline void shuffle(seq_t const & seq){
        shuffle_core(
            inc::unified_seq<seq_t>(seq)
        );
    }
}

#endif

xexport(mixc::algo_shuffle::shuffle)
