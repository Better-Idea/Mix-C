#pragma once
#include"define/base_type.hpp"

namespace mixc{
    template<uxx>
    struct unsigned_type{};
    template<> struct unsigned_type<1> { using result = u08; };
    template<> struct unsigned_type<2> { using result = u16; };
    template<> struct unsigned_type<4> { using result = u32; };
    template<> struct unsigned_type<8> { using result = u64; };
}
