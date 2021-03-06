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
#include"meta/is_same.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_trim_start{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        auto trim_start(inc::initializer_list<item_t> values, alloc_t const & alloc) const {
            auto token  = the_t(values.begin(), values.size());
            auto r      = the;

            if (auto index = the.index_of_first_miss(values); index != not_exist){
                r       = r.backward(index);
            }
            if constexpr (inc::is_same<alloc, decltype(nullptr)>){
                r       = r.clone(alloc);
            }
            return r;
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = core<item_t>;

        final_t trim_start(item_t value) const {
            return the.trim_start({ value }, nullptr);
        }

        final_t trim_start(inc::initializer_list<item_t> values) const {
            return the.trim_start(values, nullptr);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t trim_start(item_t value, alloc_t const & alloc) const {
            return the.trim_start({ value }, alloc);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t trim_start(inc::initializer_list<item_t> values, alloc_t const & alloc) const {
            return the.trim_start(values, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_trim_start::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_start::xuser
