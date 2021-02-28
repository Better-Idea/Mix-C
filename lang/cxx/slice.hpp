#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_slice
#define xpack_lang_cxx_slice
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_slice::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx.hpp"
#include"macro/xdebug_fail.hpp"
#include"math/index_system.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_slice{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<can_interval interval_t>
        auto slice(interval_t const & range) const {
            range.normalize(the.length());
            auto left = range.left();
            auto right = range.right();
            xdebug_fail(left > right);
            return the.backward(left).length(right - left + 1);
        }

        template<can_interval interval_t, class alloc_t>
        auto slice(interval_t const & range, alloc_t const & alloc) const {
            range.normalize(the.length());
            ixx left  = ixx(range.left());
            ixx right = ixx(range.right());

            uxx         target_length;
            item_t *    buf;
            item_t *    temp;

            if (left <= right) {
                target_length   = uxx(right - left + 1);
                temp            = buf = alloc(target_length);
                while(left <= right){
                    temp[0]     = the[uxx(left)];
                    temp       += 1;
                    left       += 1;
                }
            }
            else{
                target_length   = uxx(left - right + 1);
                temp            = buf = alloc(target_length);
                while(left >= right){
                    temp[0]     = the[uxx(left)];
                    temp       += 1;
                    left       -= 1;
                }
            }
            return the_t(buf, target_length);
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = core<item_t>;

        template<can_interval interval_t>
        final_t slice(interval_t const & range) const {
            return the.slice(range);
        }

        template<can_interval interval_t, class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t slice(interval_t const & range, alloc_t const & alloc) const {
            return the.slice(range, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_slice::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_slice::xuser
