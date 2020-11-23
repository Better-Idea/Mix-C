#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_compare_fastly
#define xpack_lang_cxx_compare_fastly
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_compare_fastly::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_compare_fastly{
    template<class item_t>
    struct core : inc::cxx<item_t> {
        using base_t = inc::cxx<item_t>;
        using base_t::base_t;
        using the_t = core<item_t>;

        core(base_t const & self) : 
            base_t(self){}

    
        ixx compare_fastly(the_t value, inc::can_compare<item_t> compare) const {
            if (ixx r = ixx(the.length() - value.length()); r != 0){
                return r;
            }
            for(uxx i = 0; i < the.length(); i++){
                if (ixx r = compare(the[i], value[i]); r != 0){
                    return r;
                }
            }
            return 0;
        }
    };

    template<class final, class base, class item_t>
    struct meta : base {
        using base::base;
        using the_t = core<item_t>;

        ixx compare_fastly(
            final                  value, 
            inc::can_compare<item_t> compare = inc::default_compare<item_t>) const {
            return the.compare_fastly(value, compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_compare_fastly::xuser{
    template<class final, class item_t>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_compare_fastly::xuser
