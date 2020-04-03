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
            uxxp ptr  = uxxp(mem);
            uxx  r    = 0;
            uxx  mask = rest * 8/*bit*/ - 1;
            rest      = ptr[blocks] & mask;

            for(auto i = 0; i <= blocks; i++){
                auto plus = i == blocks ? rest : ptr[i];
                r        += plus;
                r         = inc::ring_shift_left(r, plus);
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
