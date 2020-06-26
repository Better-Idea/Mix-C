#ifndef xpack_multi_barrier
#define xpack_multi_barrier
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::multi_barrier
    #include"define/base_type.hpp"
    #include"lock/atom_fetch_add.hpp"
    #include"lock/atom_fetch_sub.hpp"
    #include"macro/xgc.hpp"

    namespace xuser::origin{
        constexpr uxx primer_number_key[] = {
            59369, 59377, 59387, 59393, 59399, 59407, 59417, 59419, 59441, 59443, 59447, 59453, 59467, 59471, 59473, 59497, 59509, 59513, 59539, 59557, 59561, 59567, 59581, 59611, 59617, 59621, 59627, 59629, 59651, 59659, 59663, 59669, 59671, 59693, 59699, 59707, 59723, 59729, 59743, 59747, 59753, 59771, 59779, 59791, 59797, 59809, 59833, 59863, 59879, 59887, 59921, 59929, 59951, 59957, 59971, 59981, 59999, 60013, 60017, 60029, 60037, 60041, 60077, 60083, 60089, 60091, 60101, 60103, 60107, 60127, 60133, 60139, 60149, 60161, 60167, 60169, 60209, 60217, 60223, 60251, 60257, 60259, 60271, 60289, 60293, 60317, 60331, 60337, 60343, 60353, 60373, 60383, 60397, 60413, 60427, 60443, 60449, 60457, 60493, 60497,
        };

        enum class lock_state_t{
            accept,
            blocked,
        };

        struct multi_barrier{
            xgc_fields(
                xiam(multi_barrier),
            );
            uxx primer_sum = 0;
        public:
            lock_state_t try_lock(uxx primer_type){
                if (primer_sum % primer_type){
                    return lock_state_t::blocked;
                }
                if (inc::atom_fetch_add(& primer_sum, primer_type) == 0 or 
                    primer_sum % primer_type == 0){
                    return lock_state_t::accept;
                }
                else{
                    inc::atom_fetch_sub(& primer_sum, primer_type);
                    return lock_state_t::blocked;
                }
            }

            void unlock(uxx primer_type){
                inc::atom_fetch_sub(& primer_sum, primer_type);
            }
        };
    }
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::multi_barrier::origin;
}
