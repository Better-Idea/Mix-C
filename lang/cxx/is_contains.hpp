#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_is_contains
#define xpack_lang_cxx_is_contains
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_is_contains::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_is_contains{
    template<class final_t, class base, class item_t>
    struct meta : base{
        using base::base;
        using the_t = inc::cxx<item_t>;
        using default_cmp_t = decltype(inc::default_compare<item_t>);

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_contains(item_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first(value, compare) != not_exist;
        }

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_contains(inc::initializer_list<item_t> values, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first(values, compare) != not_exist;
        }

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_contains(final_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first(value, compare) != not_exist;
        }
    };
}

#endif

namespace mixc::lang_cxx_is_contains::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_is_contains::xuser
