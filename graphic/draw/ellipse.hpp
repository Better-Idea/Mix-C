#ifndef xpack_extern_graphic_draw_ellipse
#define xpack_extern_graphic_draw_ellipse
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_ellipse::inc
#include"define/base_type.hpp"
#include"graphic/draw/ellipse_quarter.hpp"
#include"macro/xexport.hpp"
#include"math/point.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_ellipse::origin{
    template<class rander_invoke>
    inline void ellipse(inc::pt2wx p, uxx rx, uxx ry, rander_invoke const & rander){
        inc::draw::ellipse_quarter(rx, ry, [&](auto x, auto y){
            rander(p.x() + x, p.y() + y);
            rander(p.x() - x, p.y() + y);
            rander(p.x() + x, p.y() - y);
            rander(p.x() - x, p.y() - y);
        });
    }

    template<class rander_invoke>
    inline void ellipse(uxx rx, uxx ry, rander_invoke const & rander){
        ellipse({}, rx, ry, rander);
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_ellipse::origin)
