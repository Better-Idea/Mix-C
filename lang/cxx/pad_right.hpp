#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_pad_right
#define xpack_lang_cxx_pad_right
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_pad_right::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx.hpp"
#include"memop/copy.hpp"
#include"memop/fill.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_pad_right{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto pad_right(uxx count, item_t value, alloc_t const & alloc) const {
            uxx                     length = the.length() + count;
            the_t                   r{ alloc(length), length };
            inc::copy_with_operator(r, the, the.length());
            inc::fill_with_operator(r.backward(the.length()), value, count);
            return r;
        }
    };

    template<class final_t, class base, class item_t>
    struct meta : base {
        using base::base;
        using the_t = core<item_t>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t pad_right(uxx count, item_t value, alloc_t const & alloc) const {
            return the.pad_right(count, value, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_pad_right::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_pad_right::xuser
