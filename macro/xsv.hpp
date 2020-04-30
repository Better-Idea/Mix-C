#ifndef xpack_macro_xsv
#define xpack_macro_xsv
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xsv
        #include"configure.hpp"
        #include"define/base_type.hpp"
        #include"gc/private/token.hpp"
        #include"memory/new.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xsv{
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
            new (this) ::mixc::gc_token::token_plus(sizeof(str) / sizeof(item) - 1);\
        }                                                                           \
        item buf[sizeof(padding str) / sizeof(item)];                               \
    };                                                                              \
    static sv   __mem__;                                                            \
    static sv * __tmp__ = & __mem__;                                                \
    return *(mixc::macro_xsv::static_string_holder<item> *)(& __tmp__)

    #define xsv(item,padding,str)       ([]() { __xsv__(item, padding, str); }())
#endif

namespace xuser::inc{
    using ::mixc::macro_xsv::static_string_holder;
}
