#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_whitespace
#define xpack_lang_wxx_is_whitespace
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_is_whitespace::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_is_whitespace{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        bool is_whitespace() const {
            return
                the.data == 0x09 or
                the.data == 0x0a or
                the.data == 0x0b or
                the.data == 0x0c or
                the.data == 0x0d or
                the.data == 0x20 or
                the.data == 0x85 or
                the.data == 0xa0;
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        bool is_whitespace() const {
            return the.is_whitespace();
        }

        final_t & is_whitespace(bool * result) const {
            result[0] = the.is_whitespace();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_is_whitespace::xuser{
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_whitespace::xuser
