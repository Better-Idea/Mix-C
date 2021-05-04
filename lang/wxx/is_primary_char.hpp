#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_primary_char
#define xpack_lang_wxx_is_primary_char
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_is_primary_char::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_is_primary_char{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        bool is_primary_char() const {
            if constexpr (sizeof(data) == sizeof(char)){
                return (u08(data) & 0x80) != 0;
            }
            return true;
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        bool is_primary_char() const {
            return the.is_primary_char();
        }

        final_t & is_primary_char(bool * result) const {
            result[0] = the.is_primary_char();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_is_primary_char::xuser {
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_primary_char::xuser