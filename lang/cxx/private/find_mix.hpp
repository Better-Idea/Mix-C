#ifndef xpack_lang_cxx_private_find_mix
#define xpack_lang_cxx_private_find_mix
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_cxx_private_find_mix::inc
#include"interface/can_compare.hpp"
#include"lang/cxx/private/find.hpp"
#include"lang/cxx/private/find_reserve.hpp"
#include"lang/cxx.hpp"
#include"macro/xitr_foreach.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_private_find_mix::origin{
    template<class item_t, uxx is_reserve_v>
    struct find_mix{
        using the_t = inc::cxx<item_t>;

        template<class cmp_t>
        static uxx invoke(
            the_t               cur, 
            the_t               value, 
            ixx                 n_th, 
            cmp_t       const & compare = inc::default_compare<item_t>
        ) {
            constexpr auto mode     = inc::itr_has_loop_control | inc::itr_has_index;
            struct{
                uxx index, n;
            } expect;

            expect.index            = not_exist;
            expect.n                = uxx(n_th >= 0 ? n_th : -n_th - 1);

            auto match              = [&](uxx the_n_th, uxx index){
                if (expect.n == the_n_th){
                    expect.index    = index;
                    return loop_t::finish;
                }
                return loop_t::go_on;
            };

            if (uxx(is_reserve_v) ^ uxx(n_th >= 0)){
                inc::find<item_t, mode>::invoke(cur, value, match, compare);
            }
            else{
                inc::find_reserve<item_t, mode>::invoke(cur, value, match, compare);
            }
            return expect.index;
        }
    };
}

#endif

xexport_space(mixc::lang_cxx_private_find_mix::origin)
