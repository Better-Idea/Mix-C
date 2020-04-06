#ifndef xpack_algo_shuffle
#define xpack_algo_shuffle
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_shuffle
        #include"define/base_type.hpp"
        #include"macro/xrange.hpp"
        #include"macro/xref.hpp"
        #include"math/random.hpp"
        #include"memop/swap.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_shuffle{
        template<class item_t>
        inline void shuffle(inc::ranger<item_t> range){
            for (uxx i = 0, length = range.length(); i < length; i++){
                auto a = inc::random<uxx>() % length;
                auto b = inc::random<uxx>() % length; // 用两次增强随机性
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
