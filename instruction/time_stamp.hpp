#ifndef xpack_instruction_time_stamp
#define xpack_instruction_time_stamp
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_time_stamp
        #include"define/base_type.hpp"
        #include"define/platform.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_time_stamp{
        inline u64 time_stamp(){
            #if xis_x86
                u32 low;
                u32 high;
                asm("rdtsc":"=a"(low),"=d"(high));
                return u64(high) << 32 | low;
            #endif
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_time_stamp::time_stamp;
}
