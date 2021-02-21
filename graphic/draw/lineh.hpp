#ifndef xpack_extern_graphic_draw_lineh
#define xpack_extern_graphic_draw_lineh
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_lineh::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/point.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_lineh::origin{
    template<class rander_invoke>
    inline void lineh(inc::pt2wx p, uxx width_of_left_to_right, rander_invoke const & rander){
        while(width_of_left_to_right-- > 0){
            rander(p.x(), p.y());
            p = p(1, 0);
        }
    }

    template<class rander_invoke>
    inline void lineh(uxx width_of_left_to_right, rander_invoke const & rander){
        lineh({}, width_of_left_to_right, rander);
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_lineh::origin)
