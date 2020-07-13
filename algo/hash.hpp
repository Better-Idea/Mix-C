#ifndef xpack_algo_hash
#define xpack_algo_hash
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::algo_hash
    #include"define/base_type.hpp"
    #include"memop/addressof.hpp"
    #include"instruction/ring_shift_left.hpp"
    #include"instruction/count_of_set.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_hash{
        inline uxx core(voidp mem, uxx blocks, uxx rest){
            uxxp ptr  = (uxxp)mem;
            uxx  mask = (uxx(1) << (rest * 8)) - 1;
            uxx  val  = (ptr[blocks] & mask);
            uxx  x    = (val);
            uxx  y    = (magic_number * (u32(-1) >> 1));

            for(uxx i = 0; i <= blocks; i++){
                y        += i == blocks ? ptr[i] & mask : ptr[i];
                x        += inc::count_of_set(y);
                y        += inc::ring_shift_left(x, y);
                x        += inc::ring_shift_left(y, x);
            }
            return x;
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
