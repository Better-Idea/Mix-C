#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_bends
#define xpack_lang_cxx_trim_bends
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_trim_bends::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx/index_of_first_miss.hpp"
#include"lang/cxx/index_of_last_miss.hpp"
#include"lang/cxx.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_trim_bends{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class alloc_t>
        the_t trim_bends(inc::initializer_list<item_t> values, alloc_t const & alloc) const {
            auto token  = the_t(values.begin(), values.size());
            auto r      = the;

            if (auto index = the.index_of_first_miss(values); index != not_exist){
                r       = r.backward(index);
            }
            if (auto index = r.index_of_last_miss(values); index != not_exist){
                r       = r.length(index + 1);
            }
            if constexpr (not inc::is_same<alloc_t, decltype(nullptr)>){
                r       = r.clone(alloc);
            }
            return r;
        }
    };

    template<class final_t, class base, class item_t>
    struct meta : base{
        using base::base;
        using the_t = core<item_t>;

        final_t trim_bends(item_t value) const {
            return the.trim_bends({ value }, nullptr);
        }

        final_t trim_bends(inc::initializer_list<item_t> values) const {
            return the.trim_bends(values, nullptr);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t trim_bends(item_t value, alloc_t const & alloc) const {
            return the.trim_bends({ value }, alloc);
        }

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t trim_bends(inc::initializer_list<item_t> values, alloc_t const & alloc) const {
            return the.trim_bends(values, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_trim_bends::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_bends::xuser
