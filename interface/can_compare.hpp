#ifndef xpack_interface_can_compare
#define xpack_interface_can_compare
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_compare
#include"define/base_type.hpp"
#include"macro/xcmp.hpp"
#include"macro/xnew.hpp"
#include"macro/xinterface.hpp"
#include"memop/cmp.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_compare::origin{
    template<class item_t>
    xinterface(
        xtmpl(icompare, item_t),
        xoper(ixx(item_t const & left, item_t const & right))
    );

    template<class functor_t, class item_t>
    concept can_compare = requires(functor_t const & call, icompare<item_t> * ptr){
        xnew(ptr) icompare<item_t>(call);
    };

    template<class type>
    constexpr auto default_compare = 
        [] xcmp(type){
            return cmp<type>(left, right);
        };

    template<class type>
    constexpr auto default_compare_neg = 
        [] xcmp(type){
            return cmp<type>(right, left);
        };
}
#endif

xexport_space(mixc::interface_can_compare::origin)