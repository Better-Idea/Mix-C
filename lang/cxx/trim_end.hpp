#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_end
#define xpack_lang_cxx_trim_end
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_trim_end
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx/index_of_last_miss.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_trim_end{
    template<class item>
    struct core : inc::cxx<item> {
        using base_t = inc::cxx<item>;
        using base_t::base_t;
        using the_t = core<item>;

        core(base_t const & self) : 
            base_t(self){}

        auto trim_end(inc::initializer_list<item> values, inc::can_alloc<item> alloc) {
            auto token  = the_t(values.begin(), values.size());
            auto r      = the;

            if (auto index = r.index_of_last_miss(values); index != not_exist){
                r       = r.length(index + 1);
            }
            if (alloc != nullptr){
                r       = r.clone(alloc);
            }
            return r;
        }
    };

    template<class final, class base, class item>
    struct meta : base{
        using base::base;
        using the_t = core<item>;

        final trim_end(item value, inc::can_alloc<item> alloc = nullptr) {
            return the.trim_end({ value }, alloc);
        }

        final trim_end(inc::initializer_list<item> values, inc::can_alloc<item> alloc = nullptr) {
            return the.trim_end(values, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_trim_end::xuser{
    template<class final, class item>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item>, item>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_end::xuser
