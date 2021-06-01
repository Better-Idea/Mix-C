#ifndef xpack_macro_xsv
#define xpack_macro_xsv
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xsv::inc
#include"configure/switch.hpp"
#include"define/base_type.hpp"
#include"gc/private/token.hpp"
#include"macro/xnew.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xsv{
    template<class item_t>
    struct static_string_holder {
    private:
        mixc::gc_private_token::origin::token_plus * info;
    public:
        item_t * ptr() const {
            return (item_t *)(info + 1);
        }
        uxx length() const {
            return info->length;
        }
    };
}

#define __xsv__(item_t,padding,str)                                                 \
struct sv{                                                                          \
    sv() : buf(padding str) {                                                       \
        xnew (this) ::mixc::gc_token::token_plus(sizeof(str) / sizeof(item_t) - 1); \
    }                                                                               \
    item_t buf[sizeof(padding str) / sizeof(item_t)];                               \
};                                                                                  \
static sv   __mem__;                                                                \
static sv * __tmp__ = & __mem__;                                                    \
return *(mixc::macro_xsv::static_string_holder<item_t> *)(& __tmp__)

#define xsv(item_t,padding,str)       ([]() { __xsv__(item_t, padding, str); }())
#endif

xexport(mixc::macro_xsv::static_string_holder)