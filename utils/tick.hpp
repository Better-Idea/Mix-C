#ifndef xpack_utils_tick
#define xpack_utils_tick
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_tick::inc
#include"define/base_type.hpp"
#include"instruction/time_stamp.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_tick::origin{
    xstruct(
        xname(tick),
        xprif(m_value, u64)
    )
        tick() : m_value(0ULL){
            restart();
        }

        void restart(){
            m_value = inc::time_stamp();
        }

        operator u64 () const {
            u64 clocks = inc::time_stamp() - m_value;
            return clocks;
        }
    $
}

#endif

xexport_space(mixc::utils_tick::origin)
