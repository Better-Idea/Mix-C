#ifndef xpack_extern_graphic_draw_line
#define xpack_extern_graphic_draw_line
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_graphic_draw_line::inc
#include"define/base_type.hpp"
#include"graphic/draw/lineh.hpp"
#include"graphic/draw/linev.hpp"
#include"macro/xexport.hpp"
#include"math/abs.hpp"
#include"math/point.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_graphic_draw_line::origin{
    template<class rander_invoke>
    inline void line(inc::pt2wx p0, inc::pt2wx p1, rander_invoke const & rander){
        auto dis = p1 - p0;

        if (dis.x() == 0){
            if (dis.y() > 0){
                return inc::draw::linev(p0,  dis.y(),  rander);
            }
            else{
                return inc::draw::linev(p1, -dis.y(), rander);
            }
        }

        if (dis.y() == 0){
            if (dis.x() > 0){
                return inc::draw::linev(p0,  dis.x(), rander);
            }
            else{
                return inc::draw::linev(p1, -dis.x(), rander);
            }
        }

        enum{ half = sizeof(uxx) * 4 };
        ixx x           = p0.x();
        ixx y           = p0.y();
        ixx absx        = inc::abs(dis.x());
        ixx absy        = inc::abs(dis.y());
        ixx endx        = p1.x();
        ixx endy        = p1.y();

        if (absx <= absy){
            ixx k       = (ixx(dis.x()) << half) / absy;
            ixx sk      = (ixx(p0.x()) << half);
            ixx step_y  = (dis.y() > 0 ? 1 : -1);

            do{
                rander(uxx(x), uxx(y));
                sk     += k;
                x       = sk >> half;
                y      += step_y;
            }while(y != p1.y());
        }
        else{
            ixx k       = (ixx(dis.y()) << half) / absx;
            ixx sk      = (ixx(p0.y()) << half);
            ixx step_x  = (dis.x() > 0 ? 1 : -1);

            do{
                rander(uxx(x), uxx(y));
                sk     += k;
                y       = sk >> half;
                x      += step_x;
            }while(x != p1.x());
        }
    }
}
#endif

xexport_spacex(draw, mixc::extern_graphic_draw_line::origin)
