#ifndef xpack_algo_shuffle
#define xpack_algo_shuffle
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_shuffle
        #include"define/base_type.hpp"
        #include"interface/ranger.hpp"
        #include"math/random.hpp"
        #include"memop/addressof.hpp"
        #include"memop/swap.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_shuffle{
        template<class item_t>
        inline void shuffle(inc::ranger<item_t> const & range){
            uxx length  = range.length();
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
                    xref range[a],
                    xref range[b]
                );
            }
            else for(; i < length; i++){
                a = inc::random<uxx>() % length;
                b = inc::random<uxx>() % length;

                inc::swap(
                    xref range[a],
                    xref range[b]
                );
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_shuffle::shuffle;
}
