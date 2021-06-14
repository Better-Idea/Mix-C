#ifndef xpack_memop_bytes_store
#define xpack_memop_bytes_store
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_bytes_store::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/is_integer.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_bytes_store{
    template<inc::is_integer int_t>
    inline uxx bytes_store(void * buffer, int_t value, uxx bytes = sizeof(int_t)){
        using u_t       = inc::unsigned_type<int_t>;
        u08p  ptr       = u08p(buffer);
        u_t   v         = u_t(value);

        for(uxx i = 0; i < bytes; i++){
            ptr[i]      = u08(v);
            v         >>= 8;
        }
        return bytes;
    }
}

#endif

xexport(mixc::memop_bytes_store::bytes_store)