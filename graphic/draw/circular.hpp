#ifndef xpack_extern_graphic_draw_circular
#define xpack_extern_graphic_draw_circular
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_circular::inc
#include"define/base_type.hpp"
#include"graphic/draw/circular_eighth.hpp"
#include"macro/xexport.hpp"
#include"math/point.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_circular::origin{
    template<class rander_invoke>
    inline void circular(inc::pt2wx p, uxx r, rander_invoke const & rander){
        circular_eighth(r, [&](auto x, auto y){
            rander(p.x() + x, p.y() + y);
            rander(p.x() + x, p.y() - y);
            rander(p.x() - x, p.y() + y);
            rander(p.x() - x, p.y() - y);
            rander(p.x() + y, p.y() + x);
            rander(p.x() - y, p.y() + x);
            rander(p.x() + y, p.y() - x);
            rander(p.x() - y, p.y() - x);
        });
    }

    template<class rander_invoke>
    inline void circular(uxx r, rander_invoke const & rander){
        circular({}, r, rander);
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_circular::origin)
