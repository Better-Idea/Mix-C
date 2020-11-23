#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_pad_bends
#define xpack_lang_cxx_pad_bends
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_pad_bends::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx.hpp"
#include"memop/copy.hpp"
#include"memop/fill.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_pad_bends{
    template<class item>
    struct core : inc::cxx<item> {
        using base_t = inc::cxx<item>;
        using base_t::base_t;
        using the_t = core<item>;

        core(base_t const & self) : 
            base_t(self){}

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item>
        )
        auto pad_bends(uxx left_count, uxx right_count, item value, alloc_t const & alloc) const {
            uxx                     total_length = left_count + right_count + the.length();
            the_t                   r(alloc(total_length), total_length);
            inc::fill_with_operator(r, value, left_count);
            inc::copy_with_operator(r.backward(left_count), the, the.length());
            inc::fill_with_operator(r.backward(left_count + the.length()), value, right_count);
            return r;
        }
    };

    template<class final, class base, class item>
    struct meta : base {
        using base::base;
        using the_t = core<item>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item>
        )
        final pad_bends(uxx left_count, uxx right_count, item value, alloc_t const & alloc) const {
            return the.pad_bends(left_count, right_count, value, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_pad_bends::xuser {
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_pad_bends::xuser
