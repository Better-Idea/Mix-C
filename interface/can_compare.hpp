#ifndef xpack_interface_can_compare
#define xpack_interface_can_compare
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_compare::inc
#include"define/base_type.hpp"
#include"interface/can_callback.hpp"
#include"macro/xcmp.hpp"
#include"memop/cmp.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_compare::origin{
    template<class item_t>
    using icompare = inc::icallback<
        ixx(item_t const & left, item_t const & right)
    >;

    template<class functor_t, class item_t>
    concept can_compare = inc::can_callback<functor_t, ixx(item_t const &, item_t const &)>;

    template<class type>
    constexpr auto default_compare = 
        [] xcmp(type){
            return inc::cmp_with_operator<type>(left, right);
        };

    template<class type>
    constexpr auto default_compare_neg = 
        [] xcmp(type){
            return inc::cmp_with_operator<type>(right, left);
        };
}
#endif

xexport_space(mixc::interface_can_compare::origin)