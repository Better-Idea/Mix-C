#ifndef xpack_draft_isa
#define xpack_draft_isa
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_2d::inc
#include"math/abs.hpp"
#include"math/point.hpp"
#include"math/round.hpp"
#include"memop/swap.hpp"
#pragma pop_macro("xuser")

namespace mixc::draft_2d::origin{
    using stdpt = inc::pt2wx;

    template<class rander_invoke>
    inline void draw_quarter_ellipse(uxx rx, uxx ry, rander_invoke const & rander){
        ixx rxx     = ixx(rx * rx);
        ixx ryy     = ixx(ry * ry);
        ixx rx2     = ixx(rxx * 2);
        ixx ry2     = ixx(ryy * 2);
        ixx x       = 0;
        ixx y       = ry;
        ixx px      = 0;
        ixx py      = rx2 * y;
        ixx p       = (ixx)inc::round_unsafe(f32(ryy + rxx * ry) + (0.25f * rxx));

        for(rander(x, y); px < py; rander(x, y)){
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

        for(p = inc::round_unsafe(f32(ryy) * (0.5 + x) * (0.5 + x) + rxx * (y - 1) * (y - 1) - rxx * ryy);
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
    inline void draw_ellipse(stdpt p, uxx rx, uxx ry, uxx thickness, rander_invoke const & rander){
        auto randerx = [&](auto x, auto y){
            rander(p.x() + x, p.y() + y);
            rander(p.x() - x, p.y() + y);
            rander(p.x() + x, p.y() - y);
            rander(p.x() - x, p.y() - y);
        };

        draw_quarter_ellipse(rx, ry, [&](auto x, auto y){
            widenv(stdpt{}.x(x).y(y), randerx);
        });
    }


    template<class rander_invoke>
    inline void draw_eight_circular(uxx r, rander_invoke const & rander){
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
    inline void draw_circular(stdpt p, uxx r, rander_invoke const & rander){
        draw_eight_circular(r, [&](auto x, auto y){
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
    inline void draw_linev(stdpt p, uxx height_of_bottom_to_top, rander_invoke const & rander){
        while(height_of_bottom_to_top-- > 0){
            rander(p.x(), p.y());
            p = p(0, 1);
        }
    }

    template<class rander_invoke>
    inline void draw_lineh(stdpt p, uxx width_of_left_to_right, rander_invoke const & rander){
        while(width_of_left_to_right-- > 0){
            rander(p.x(), p.y());
            p = p(1, 0);
        }
    }

    template<class rander_invoke>
    inline void draw_line(stdpt p0, stdpt p1, rander_invoke const & rander){
        auto dis = p1 - p0;

        if (dis.x() == 0){
            if (dis.y() > 0){
                return draw_linev(p0,  dis.y(),  rander);
            }
            else{
                return draw_linev(p1, -dis.y(), rander);
            }
        }

        if (dis.y() == 0){
            if (dis.x() > 0){
                return draw_linev(p0,  dis.x(), rander);
            }
            else{
                return draw_linev(p1, -dis.x(), rander);
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

xexport_space(mixc::draft_2d::origin)