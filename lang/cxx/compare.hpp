#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_compare
#define xpack_lang_cxx_compare
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_compare::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx.hpp"
#include"math/min.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_compare{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class cmp_t>
        ixx compare(the_t value, cmp_t const & compare) const {
            uxx len = inc::min(the.length(), value.length());

            for(uxx i = 0; i < len; i++){
                if (ixx r = compare(the[i], value[i]); r != 0){
                    return r;
                }
            }
            return ixx(the.length() - value.length());
        }
    };

    template<class final, class base, class item_t>
    struct meta : base {
        using base::base;
        using the_t = core<item_t>;

        template<class cmp_t = decltype(inc::default_compare<item_t>)>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        ixx compare(final value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.compare(value, compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_compare::xuser{
    template<class final, class item_t>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_compare::xuser
