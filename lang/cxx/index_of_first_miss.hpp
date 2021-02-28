#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_first_miss
#define xpack_lang_cxx_index_of_first_miss
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_index_of_first_miss::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_index_of_first_miss{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class cmp_t>
        uxx index_of_first_miss(item_t const * values, uxx length, cmp_t const & compare) const {
            the_t token{ values, length };

            for (uxx i = 0; i < the.length(); i++){
                if (token.index_of_first(the[i], compare) == not_exist){
                    return i;
                }
            }
            return not_exist;
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t         = core<item_t>;
        using default_cmp_t = decltype(inc::default_compare<item_t>);

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx index_of_first_miss(item_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first_miss(& value, 1, compare);
        }

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx index_of_first_miss(inc::initializer_list<item_t> values, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first_miss(values.begin(), values.size(), compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_index_of_first_miss::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_first_miss::xuser
