#ifndef xpack_utils_tick
#define xpack_utils_tick
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_tick::inc
#include"define/base_type.hpp"
#include"instruction/time_stamp.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_tick::origin{
    xstruct(
        xname(tick),
        xprif(value, u64)
    )
        constexpr tick() : value(0){
            restart();
        }

        void restart(){
            value = inc::time_stamp();
        }

        operator u64 () const {
            u64 clocks = inc::time_stamp() - value;
            return clocks;
        }
    $
}

#endif

xexport_space(mixc::utils_tick::origin)
