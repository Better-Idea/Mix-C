#pragma once
#include"gc/private/ref.hpp"
#define __xsv__(ret,item,padding,str)                                           \
struct sv{                                                                      \
    sv() : buf(padding str) {                                                   \
        new (this) plus_length(sizeof(str) / sizeof(item) - 1);                 \
    }                                                                           \
    item buf[sizeof(padding str) / sizeof(item)];                               \
};                                                                              \
static sv   __mem__;                                                            \
static sv * __tmp__ = & __mem__;                                                \
return *(ret *)(& __tmp__)

#define xsv(ret,item,padding,str)                                               \
([]() -> ret & {                                                                \
    using namespace mixc::inner_gc;                                             \
    if constexpr (is_64bit){                                                    \
        __xsv__(ret, item, padding padding, str);                               \
    }                                                                           \
    if constexpr(is_32bit){                                                     \
        __xsv__(ret, item, padding, str);                                       \
    }                                                                           \
    static_assert(sizeof(plus_length) == (is_64bit ? 16 : 8));                  \
}())
