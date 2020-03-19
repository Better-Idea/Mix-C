#pragma once
#include"define/base_type.hpp"
#include"define/platform.hpp"

namespace mixc{
    u64 time_stamp(){
        #if xis_x86
            u32 low;
            u32 high;
            asm("rdtsc":"=a"(low),"=d"(high));
            return u64(high) << 32 | low;
        #endif
    }
}
