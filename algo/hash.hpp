#ifndef xpack_algo_hash
#define xpack_algo_hash
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_hash
        #include"define/base_type.hpp"
        #include"memop/addressof.hpp"
        #include"instruction/ring_shift_left.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_hash{
        inline uxx core(voidp mem, uxx blocks, uxx rest){
            constexpr uxx ax = magic_number * ((uxx(1) << 31) - 1);
            uxxp ptr  = (uxxp)mem;
            uxx  mask = (uxx(1) << (rest * 8)) - 1;
            uxx  val  = (ptr[blocks] & mask);
            uxx  r    = (val + ax);

            for(uxx i = 0; i <= blocks; i++){
                auto plus = i == blocks ? rest : ptr[i];
                r        += inc::ring_shift_left(plus, r);
            }
            return r;
        }

        template<class type>
        inline uxx hash(type const & value){
            return core(inc::addressof(value), sizeof(type) / sizeof(uxx), sizeof(type) % sizeof(uxx));
        }
    }
#endif

namespace xuser::inc{
    using ::mixc::algo_hash::hash;
}
