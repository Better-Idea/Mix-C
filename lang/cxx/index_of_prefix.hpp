#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_prefix
#define xpack_lang_cxx_index_of_prefix
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_index_of_prefix::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_index_of_prefix{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class list_t,  class cmp_t>
        uxx index_of_prefix(list_t const & values, cmp_t const & compare) const {
            uxx i = 0;

            for(const auto & prefix : values){
                if (the.length() < prefix.length() or prefix.length() == 0){
                    continue;
                }
                if (auto && content = the_t{the}.length(prefix.length());
                    content.compare_fastly(prefix, compare) == 0){
                    return i;
                }
            }
            return not_exist;
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class cmp_t = decltype(inc::default_compare<item_t>)>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx index_of_prefix(inc::initializer_list<final_t> const & values, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_prefix(values, compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_index_of_prefix::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_prefix::xuser
