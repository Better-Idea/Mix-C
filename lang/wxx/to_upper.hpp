#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_to_upper
#define xpack_lang_wxx_to_upper
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_to_upper::inc
#include"define/base_type.hpp"
#include"lang/wxx/is_lower.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_to_upper{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        auto to_upper() const {
            auto dvalue = 'a' - 'A';
            return the.is_lower() ? the.m_value - dvalue : the.m_value;
        }
    };

    template<class final_t, class base, class type>
    struct meta: base {
        using base::base;
        using the_t = core<type>;

        final_t to_upper() const {
            return the.to_upper();
        }

        final_t & to_upper(final_t * result) const {
            result[0] = the.to_upper();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_to_upper::xuser{
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_to_upper::xuser
