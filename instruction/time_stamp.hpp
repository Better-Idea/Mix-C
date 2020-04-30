#ifndef xpack_instruction_time_stamp
#define xpack_instruction_time_stamp
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_time_stamp
        #include"configure.hpp"
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_time_stamp{
        inline u64 time_stamp(){
            #if xis_x86
                return __builtin_ia32_rdtsc();
            #endif
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_time_stamp::time_stamp;
}
