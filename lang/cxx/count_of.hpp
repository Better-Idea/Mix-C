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
#include"lang/cxx/find.hpp"
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

            the.find(substr, compare, [&](uxx){
                times      += 1;
            });
            return times;
        }

        template<class cmp_t>
        uxx count_of(item_t chr, cmp_t const & compare) const {
            uxx times       = 0;

            the.find(chr, compare, [&](uxx){
                times      += 1;
            });
            return times;
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;
        using default_cmp_t = decltype(inc::default_compare<item_t>);

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx count_of(final_t substr, cmp_t const & compare = inc::default_compare<item_t>) const {
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
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_count_of::xuser
