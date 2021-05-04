#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_ascii
#define xpack_lang_wxx_is_ascii
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_is_ascii::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#include"meta/unsigned_type.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_is_ascii{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        bool is_ascii() const {
            return inc::unsigned_type<type>(data) < 128;
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        bool is_ascii() const {
            return the.is_ascii();
        }

        final_t & is_ascii(bool * result) const {
            result[0] = the.is_ascii();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_is_ascii::xuser {
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_ascii::xuser
