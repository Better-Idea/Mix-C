#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_alpha
#define xpack_lang_wxx_is_alpha
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_is_alpha::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_is_alpha{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        bool is_alpha() const {
            // 0x41 0100 0001
            // 0x61 0110 0001
            auto mask = the.m_value | 0x20;
            return 'a' <= mask && mask <= 'z';
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        bool is_alpha() const {
            return the.is_alpha();
        }

        final_t & is_alpha(bool * result) const {
            result[0] = the.is_alpha();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_is_alpha::xuser {
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_alpha::xuser
