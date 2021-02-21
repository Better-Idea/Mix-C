#ifndef xpack_extern_graphic_draw_circular_eighth
#define xpack_extern_graphic_draw_circular_eighth
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_circular_eighth::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/point.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_circular_eighth::origin{
    template<class rander_invoke>
    inline void circular_eighth(uxx r, rander_invoke const & rander){
        ixx e = ixx(1 - r);
        ixx x = 0;
        ixx y = ixx(r);

        for(rander(x, y); x <= y; x += 1, rander(x, y)){
            if (e < 0){
                e += 2 * x + 3;
            }
            else{
                e += 2 * (x - y) + 5;
                y -= 1;
            }
        }
    }

    template<class rander_invoke>
    inline void circular_eighth(inc::pt2wx p, uxx r, rander_invoke const & rander){
        circular_eighth(r, [&](uxx x, uxx y){
            rander(p(x, y));
        });
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_circular_eighth::origin)
