#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_count_of
#define xpack_lang_cxx_count_of
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_count_of::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_count_of{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class cmp_t>
        uxx count_of(the_t substr, cmp_t const & compare) const {
            uxx times       = 0;

            the.index_of_first(substr, [&](uxx){
                times      += 1;
            }, compare);
            return times;
        }

        template<class cmp_t>
        uxx count_of(item_t chr, cmp_t const & compare) const {
            uxx times       = 0;

            the.index_of_first(chr, [&](uxx){
                times      += 1;
            }, compare);
            return times;
        }
    };

    template<class final, class base, class item_t>
    struct meta : base{
        using base::base;
        using the_t = core<item_t>;
        using default_cmp_t = decltype(inc::default_compare<item_t>);

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx count_of(final substr, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.count_of(substr);
        }

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx count_of(item_t chr, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.count_of(chr, compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_count_of::xuser{
    template<class final, class item_t>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_count_of::xuser
