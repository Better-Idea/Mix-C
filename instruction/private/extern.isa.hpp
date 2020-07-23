#include<intrin.h>
#include"define/base_type.hpp"

namespace mixc::instruction_time_stamp{
    u64 rdtsc(){
        return __rdtsc();
    }
}


void atom_add(){
    
}
