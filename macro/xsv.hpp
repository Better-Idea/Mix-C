#pragma once
#include"gc/private/token.hpp"
#include"define/platform.hpp"
#include"memory/new.hpp"

namespace mixc{
    template<class item>
    struct static_string_holder {
    private:
        mixc::gc_token::token_plus * info;
    public:
        item * ptr() const {
            return (item *)(info + 1);
        }
        uxx length() const {
            return info->length;
        }
    };
}

#define __xsv__(item,padding,str)                                               \
struct sv{                                                                      \
    sv() : buf(padding str) {                                                   \
        new (this) mixc::gc_token::token_plus(sizeof(str) / sizeof(item) - 1);  \
    }                                                                           \
    item buf[sizeof(padding str) / sizeof(item)];                               \
};                                                                              \
static sv   __mem__;                                                            \
static sv * __tmp__ = & __mem__;                                                \
return *(mixc::static_string_holder<item> *)(& __tmp__)


#define xsv(item,padding,str)                                                   \
([]() {                                                                         \
    using namespace mixc::gc_token;                                             \
    if constexpr (is_64bit){                                                    \
        __xsv__(item, padding padding, str);                                    \
    }                                                                           \
    if constexpr(is_32bit){                                                     \
        __xsv__(item, padding, str);                                            \
    }                                                                           \
    static_assert(sizeof(token_plus) == (is_64bit ? 16 : 8));                   \
}())
