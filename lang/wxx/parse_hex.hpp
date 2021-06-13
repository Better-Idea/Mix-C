#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_parse_hex
#define xpack_lang_wxx_parse_hex
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_parse_hex::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_parse_hex{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        uxx parse_hex() const {
            if ('0' <= the.m_value and the.m_value <= '9'){
                return uxx(the.m_value - '0');
            }
            if (auto v = the.m_value | 0x20; 'a' <= v and v <= 'f'){
                return uxx(v - 'a' + 10);
            }
            else{
                return not_exist;
            }
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        uxx parse_hex() const {
            return the.parse_hex();
        }
    };
}

#endif

namespace mixc::lang_wxx_parse_hex::xuser {
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_parse_hex::xuser
