#ifndef xpack_extern_graphic_draw_ellipse_quarter
#define xpack_extern_graphic_draw_ellipse_quarter
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_ellipse_quarter::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"math/point.hpp"
#include"math/round.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_ellipse_quarter::origin{
    template<class rander_invoke>
    inline void ellipse_quarter(uxx rx, uxx ry, rander_invoke const & rander){
        ixx rxx     = ixx(rx * rx);
        ixx ryy     = ixx(ry * ry);
        ixx rx2     = ixx(rxx * 2);
        ixx ry2     = ixx(ryy * 2);
        ixx x       = 0;
        ixx y       = ry;
        ixx px      = 0;
        ixx py      = rx2 * y;
        ixx p       = (ixx)inc::round_unsafe(f32(ryy + rxx * ry) + (0.25f * rxx));

        for(rander(x, y - 1); px < py; rander(x, y)){
            x      += 1;
            px     += ry2;
            
            if (p < 0){
                p  += ryy + px;
            }
            else{
                y  -= 1;
                py -= rx2;
                p  += ryy + px - py;
            }
        }

        for(p = inc::round_unsafe(f32(ryy) * (0.5f + x) * (0.5f + x) + rxx * (y + 1) * (y) - rxx * ryy);
            y-- > 0;
            rander(x, y)){

            if (py -= rx2; p > 0){
                p  += rxx - py;
            }
            else{
                x  += 1;
                px += ry2;
                p  += rxx - py + px;
            }
        }
    }

    template<class rander_invoke>
    inline void ellipse_quarter(inc::pt2wx p, uxx rx, uxx ry, rander_invoke const & rander){
        ellipse_quarter(rx, ry, [&](uxx x, uxx y){
            rander(p(x, y));
        });
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_ellipse_quarter::origin)
