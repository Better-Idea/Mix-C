#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_insert
#define xpack_lang_cxx_insert
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_insert::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx.hpp"
#include"macro/xindex_rollback.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_insert{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto insert(ixx index, the_t value, alloc_t const & alloc) const {
            xindex_rollback(the.length(), index, + 1);
            uxx                     target_length = the.length() + value.length();
            the_t                   r { alloc(target_length), target_length };
            inc::copy_with_operator(r, the, index);
            inc::copy_with_operator(r.backward(index), value, value.length());
            inc::copy_with_operator(r.backward(index + value.length()), the.backward(index), the.length() - index);
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
        final_t insert(ixx index, final_t value, alloc_t const & alloc) const {
            return the.insert(index, value, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_insert::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_insert::xuser
