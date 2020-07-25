#include"configure.hpp"
#include"define/base_type.hpp"
#include<atomic>

#define xatomop(name,op)                                                    \
namespace mixc::lock_ ## name{                                              \
    u64 name(voidp a, u64 b, uxx bytes){                                    \
        switch(bytes){                                                      \
        case 1: return ((std::atomic_uint8_t  *)a)->op(uint8_t (b));        \
        case 2: return ((std::atomic_uint16_t *)a)->op(uint16_t(b));        \
        case 4: return ((std::atomic_uint32_t *)a)->op(uint32_t(b));        \
        case 8: return ((std::atomic_uint64_t *)a)->op(uint64_t(b));        \
        default: return not_exist;                                          \
        }                                                                   \
    }                                                                       \
}

xatomop(atom_add       , operator+=)
xatomop(atom_sub       , operator-=)
xatomop(atom_and       , operator&=)
xatomop(atom_or        , operator|=)
xatomop(atom_xor       , operator^=)
xatomop(atom_fetch_add , fetch_add )
xatomop(atom_fetch_sub , fetch_sub )
xatomop(atom_fetch_and , fetch_and )
xatomop(atom_fetch_or  , fetch_or  )
xatomop(atom_fetch_xor , fetch_xor )
xatomop(atom_swap      , exchange  )

#undef  xatomop
