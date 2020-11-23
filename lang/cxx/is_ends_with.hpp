#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_is_ends_with
#define xpack_lang_cxx_is_ends_with
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_is_ends_with::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_is_ends_with{
    template<class item_t>
    struct core : inc::cxx<item_t> {
        using base_t = inc::cxx<item_t>;
        using base_t::base_t;
        using the_t = core<item_t>;

        core(base_t const & self) : 
            base_t(self){}

        template<class cmp_t = decltype(inc::default_compare<item_t>)>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_ends_with(the_t value, cmp_t const & compare) const {
            if (the.length() < value.length() or value.length() == 0){
                return false;
            }
            return the.backward(
                the.length() - value.length()
            ).compare_fastly(value, compare) == 0;
        }
    };

    template<class final, class base, class item_t>
    struct meta : base {
        using base::base;
        using the_t = ::mixc::lang_cxx_is_ends_with::core<item_t>;

        template<class cmp_t = decltype(inc::default_compare<item_t>)>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_ends_with(final value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.is_ends_with(value, compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_is_ends_with::xuser{
    template<class final, class item_t>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_is_ends_with::xuser
