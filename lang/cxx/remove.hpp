#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_remove
#define xpack_lang_cxx_remove
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_remove::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx.hpp"
#include"math/index_system.hpp"
#include"memop/copy.hpp"
#include"memop/swap.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_remove{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class interval_t, class alloc_t>
        auto remove(interval_t const & range, alloc_t const & alloc) const {
            range.normalize(the.length());

            auto  left  = range.left();
            auto  right = range.right();
            auto  len   = the.length() - (right - left + 1);
            the_t r { alloc(len), len };
            inc::copy_with_operator(r       , the            , left);
            inc::copy_with_operator(r + left, the + right + 1, len - left);
            return r;
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<inc::can_interval interval_t, class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t remove(interval_t const & range, alloc_t const & alloc) const {
            return the.remove(range, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_remove::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_remove::xuser
