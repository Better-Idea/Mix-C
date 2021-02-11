#ifndef xpack_math_point
#define xpack_math_point
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_point::inc
#include"math/sqrt.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_point {
    template<class final_t, class posx_t, class posy_t = posx_t, class posz_t = void>
    xstruct(
        xtmpl(point, final_t, posx_t, posy_t, posz_t),
        xprif(px, posx_t),
        xprif(py, posy_t),
        xprif(pz, posz_t)
    ) 
        point() : 
            point(0, 0, 0){
        }

        point(posx_t x, posy_t y, posz_t z):
            px(x), py(y), pz(z){
        }

        template<class finalx_t >
        point(point<finalx_t, posx_t, posy_t, posz_t> const & self) :
            point(self.x(), self.y(), self.z()){
        }

        final_t operator()(posx_t x, posy_t y, posz_t z){
            return final_t(px + x, py + y, pz + z);
        }

        final_t operator - (final_t const & p){
            return the(posx_t(0) - p.x(), posx_t(0) - p.y(), posx_t(0) - p.z());
        }

        final_t operator + (final_t const & p){
            return the(p.x(), p.y(), p.z());
        }

        final_t & operator -= (final_t const & p){
            the = the - p;
            return the;
        }

        final_t & operator += (final_t const & p){
            the = the + p;
            return the;
        }

        xpubget_pubset(x);
        xpubget_pubset(y);
        xpubget_pubset(z);

        xpubgetx(hypot, posx_t){
            posx_t pt = (posx_t)inc::sqrt(px * px + py * py + pz * pz);
            return pt;
        }
    $

    template<class final_t, class posx_t, class posy_t>
    xstruct(
        xspec(point, final_t, posx_t, posy_t, void),
        xprif(px, posx_t),
        xprif(py, posy_t)
    ) 
        point() : 
            point(0, 0){
        }

        point(posx_t x, posy_t y):
            px(x), py(y){
        }

        template<class finalx_t >
        point(point<finalx_t, posx_t, posy_t> const & self) :
            point(self.x(), self.y()){
        }

        final_t operator()(posx_t x, posy_t y){
            return final_t(px + x, py + y);
        }

        final_t operator - (final_t const & p){
            return the(posx_t(0) - p.x(), posx_t(0) - p.y());
        }

        final_t operator + (final_t const & p){
            return the(p.x(), p.y());
        }

        final_t & operator -= (final_t const & p){
            the = the - p;
            return the;
        }

        final_t & operator += (final_t const & p){
            the = the + p;
            return the;
        }

        xpubget_pubset(x);
        xpubget_pubset(y);

        xpubgetx(hypot, posx_t){
            posx_t pt = (posx_t)inc::sqrt(px * px + py * py);
            return pt;
        }
    $
}

#endif

#define xusing_math_point ::mixc::math_point
