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
    template<class final, class base, class item>
    struct meta : base{
        using base::base;
        using the_t = inc::cxx<item>;

        template<class cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_contains(
            item          value,
            cmp_t const & compare = inc::default_compare<item>) const {
            return the.index_of_first(value, compare) != not_exist;
        }

        template<class cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_contains(
            inc::initializer_list<item> values,
            cmp_t const &               compare = inc::default_compare<item>) const {
            return the.index_of_first(values.begin(), values.size(), compare) != not_exist;
        }

        template<class cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        bool is_contains(final value, cmp_t const & compare = inc::default_compare<item>) const {
            return the.index_of_first(value, compare) != not_exist;
        }
    };
}

#endif

namespace mixc::lang_cxx_is_contains::xuser{
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_is_contains::xuser
