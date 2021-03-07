#ifndef xpack_lang_cxx_private_find
#define xpack_lang_cxx_private_find
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_cxx_private_find::inc
#include"interface/can_compare.hpp"
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
#include"macro/xitr_foreach.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_private_find::origin{
    template<class item_t, auto mode_v>
    struct find{
        using the_t     = inc::cxx<item_t>;

        template<class func_t, class cmp_t>
        static uxx invoke(
            the_t               cur,
            the_t               value, 
            func_t      const & match, 
            cmp_t       const & compare
        ) {
            uxx     match_count = 0;
            uxx     i           = 0;
            uxx     index       = 0;
            uxx     offset      = 0;

            for(;; cur = cur.backward(i + value.length()), offset += value.length()){
                if (i = cur.index_of_first(value, compare); i == not_exist){
                    break;
                }

                offset         += i;
                match_count    += 1;

                if (inc::itr_switch<mode_v>(xref index, match, offset) == loop_t::finish){
                    break;
                }
            }
            return match_count;
        }
    };
}

#endif

xexport_space(mixc::lang_cxx_private_find::origin)
