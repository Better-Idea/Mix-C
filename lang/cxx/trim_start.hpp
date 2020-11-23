#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_start
#define xpack_lang_cxx_trim_start
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_trim_start::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx/index_of_first_miss.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_trim_start{
    template<class item_t>
    struct core : inc::cxx<item_t> {
        using base_t = inc::cxx<item_t>;
        using base_t::base_t;
        using the_t = core<item_t>;

        core(base_t const & self) : 
            base_t(self){}

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        auto trim_start(inc::initializer_list<item_t> values, alloc_t const & alloc) const {
            auto token  = the_t(values.begin(), values.size());
            auto r      = the;

            if (auto index = the.index_of_first_miss(values); index != not_exist){
                r       = r.backward(index);
            }
            if (alloc != nullptr){
                r       = r.clone(alloc);
            }
            return r;
        }
    };

    template<class final, class base, class item_t>
    struct meta : base {
        using base::base;
        using the_t = core<item_t>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final trim_start(item_t value, alloc_t const & alloc = nullptr) const {
            return the.trim_start({ value }, alloc);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final trim_start(inc::initializer_list<item_t> values, alloc_t const & alloc = nullptr) const {
            return the.trim_start(values, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_trim_start::xuser {
    template<class final, class item_t>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_start::xuser
