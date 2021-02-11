#ifndef xusing_lang_wxx
#include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_digital
#define xpack_lang_wxx_is_digital
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_wxx")
#undef  xusing_lang_wxx
#undef  xuser
#define xuser mixc::lang_wxx_is_digital::inc
#include"define/base_type.hpp"
#include"lang/wxx.hpp"
#pragma pop_macro("xusing_lang_wxx")
#pragma pop_macro("xuser")

namespace mixc::lang_wxx_is_digital{
    template<class type>
    struct core : inc::wxx<type>{
        using inc::wxx<type>::wxx;
        using the_t = core<type>;

        bool is_digital() const {
            return '0' <= the.data and the.data <= '9';
        }
    };

    template<class final_t, class base, class type>
    struct meta : base {
        using base::base;
        using the_t = core<type>;

        bool is_digital() const {
            return the.is_digital();
        }

        final_t & is_digital(bool * result) const {
            result[0] = the.is_digital();
            return thex;
        }
    };
}
#endif

namespace mixc::lang_wxx_is_digital::xuser{
    template<class final_t, class item_t>
    using wxx = meta<final_t, xusing_lang_wxx::wxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_digital::xuser
