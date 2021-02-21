#ifndef xpack_extern_graphic_draw_linev
#define xpack_extern_graphic_draw_linev
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_linev::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/point.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_linev::origin{
    template<class rander_invoke>
    inline void lineh(inc::pt2wx p, uxx width_of_left_to_right, rander_invoke const & rander){
        while(width_of_left_to_right-- > 0){
            rander(p.x(), p.y());
            p = p(1, 0);
        }
    }

    template<class rander_invoke>
    inline void linev(uxx height_of_bottom_to_top, rander_invoke const & rander){
        linev({}, height_of_bottom_to_top, rander);
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_linev::origin)
