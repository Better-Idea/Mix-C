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
    template<class item>
    struct core : inc::cxx<item> {
        using base_t = inc::cxx<item>;
        using base_t::base_t;
        using the_t = core<item>;

        core(base_t const & self) : 
            base_t(self){}

        auto slice(iinterval range) const {
            range.normalize(the.length());
            auto left = range.left();
            auto right = range.right();
            xdebug_fail(left > right);
            return the.backward(left).length(right - left + 1);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item>
        )
        auto slice(iinterval range, alloc_t const & alloc) const {
            range.normalize(the.length());
            ixx left  = ixx(range.left());
            ixx right = ixx(range.right());

            uxx    target_length;
            item * buf;
            item * temp;

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

    template<class final, class base, class item>
    struct meta : base {
        using base::base;
        using the_t = core<item>;

        final slice(iinterval range) const {
            return the.slice(range);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item>
        )
        final slice(iinterval range, alloc_t const & alloc) const {
            return the.slice(range, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_slice::xuser {
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_slice::xuser
