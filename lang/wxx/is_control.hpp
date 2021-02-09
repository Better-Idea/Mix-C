#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_control
#define xpack_lang_wxx_is_control
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_is_control::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_is_control{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        bool is_control() const {
            // 9f 1001 1111
            // 7f 0111 1111
            // 1f 0001 1111
            return the.data <= 0x1f or (0x7f <= the.data and the.data <= 0x9f);
        }
    };

    template<class final, class base, class type>
    struct meta : base {
        using base::base;
        using the_t = core<type>;

        bool is_control() const {
            return the.is_control();
        }

        final & is_control(bool * result) const {
            result[0] = the.is_control();
            return thex;
        }
    };
}

#endif

namespace mixc::lang_wxx_is_control::xuser{
    template<class final, class item_t>
    using wxx = meta<final, xusing_lang_wxx::wxx<final, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_control::xuser

