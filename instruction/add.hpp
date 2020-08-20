#ifndef xpack_instruction_add
#define xpack_instruction_add
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_add
#include"configure.hpp"
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::instruction_add{
    inline u128 add(u64 a, u64 b, u64 cf){
        u128 r{};

        #if xis_x86
            #if xis_os64
                r.high   = __builtin_ia32_addcarryx_u64(u08(cf), a, b, & r.low);
            #elif xis_os32
                u32 cs[2];
                u32 as[] = { u32(a), u32(a >> 32) };
                u32 bs[] = { u32(b), u32(b >> 32) };
                cf       = __builtin_ia32_addcarryx_u32(u08(cf), as[0], bs[0], & cs[0]);
                cf       = __builtin_ia32_addcarryx_u32(u08(cf), as[1], bs[1], & cs[1]);
                r.low    = cs[0] | (cs[1] << 32);
                r.high   = cf;
            #endif
        #endif
        return r;
    }

    inline u128 add(u64 a, u64 b){
        return add(a, b, 0);
    }
}

#endif

namespace xuser::inc{
    using ::mixc::instruction_add::add;
}
