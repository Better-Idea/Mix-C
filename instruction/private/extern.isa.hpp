#include"configure.hpp"
#include"define/base_type.hpp"

#if xis_windows
#include<intrin.h>
namespace mixc::instruction_time_stamp{
    u64 rdtsc(){
        return __rdtsc();
    }
}
#endif
