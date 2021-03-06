#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_reverse
#define xpack_lang_cxx_reverse
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_reverse::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx/slice.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_reverse{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto reverse(alloc_t const & alloc) const {
            return the.slice(oc{-1, 0}, alloc);
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t reverse(alloc_t const & alloc) const {
            return the.reverse(alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_reverse::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_reverse::xuser
