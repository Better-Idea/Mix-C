#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_align_left
#define xpack_lang_cxx_align_left
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_align_left::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx/pad_right.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_align_left{
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
        auto align_left(uxx width, item pad, alloc_t const & alloc) const {
            if (ixx r = ixx(the.length() - width); r >= 0){
                return the.shorten(uxx(r)).clone(alloc);
            }
            else{
                return the.pad_right(uxx(-r), pad, alloc);
            }
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
        final align_left(uxx width, alloc_t const & alloc) const {
            return the.align_left(width, ' ', alloc);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item>
        )
        final align_left(uxx width, item pad, alloc_t const & alloc) const {
            return the.align_left(width, pad, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_align_left::xuser{
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_align_left::xuser
