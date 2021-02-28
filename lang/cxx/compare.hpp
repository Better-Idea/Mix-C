#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_compare
#define xpack_lang_cxx_compare
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_compare::inc
#include"define/base_type.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx.hpp"
#include"math/min.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_compare{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class cmp_t>
        ixx compare(the_t value, cmp_t const & compare) const {
            uxx len = inc::min(the.length(), value.length());

            for(uxx i = 0; i < len; i++){
                if (ixx r = compare(the[i], value[i]); r != 0){
                    return r;
                }
            }
            return ixx(the.length() - value.length());
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t {
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class cmp_t = decltype(inc::default_compare<item_t>)>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        ixx compare(final_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.compare(value, compare);
        }

        #define xa_args_list       final_t const & left, final_t const & right
        #define xa_invoke          left.compare_fastly(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"

        #define xa_args_list       item_t const * left, final_t const & right
        #define xa_invoke          final_t{left}.compare_fastly(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"

        #define xa_args_list       final_t const & left, item_t const * right
        #define xa_invoke          left.compare_fastly(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"
    };
}

#endif

namespace mixc::lang_cxx_compare::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_compare::xuser
