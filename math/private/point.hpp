#ifndef xpack_math_point
#define xpack_math_point
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_point::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"math/sqrt.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_point {
    template<class final_t, class posx_t, class posy_t = posx_t, class posz_t = void>
    xstruct(
        xtmpl(point, final_t, posx_t, posy_t, posz_t),
        xprif(m_x, posx_t),
        xprif(m_y, posy_t),
        xprif(m_z, posz_t)
    ) 
        constexpr point() : 
            point(0, 0, 0){
        }

        constexpr point(posx_t x, posy_t y, posz_t z):
            m_x(x), m_y(y), m_z(z){
        }

        template<class finalx_t >
        constexpr point(point<finalx_t, posx_t, posy_t, posz_t> const & self) :
            point(self.x(), self.y(), self.z()){
        }

        final_t operator()(posx_t x, posy_t y, posz_t z){
            return final_t(m_x + x, m_y + y, m_z + z);
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
            posx_t pt = (posx_t)inc::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
            return pt;
        }
    $

    template<class final_t, class posx_t, class posy_t>
    xstruct(
        xspec(point, final_t, posx_t, posy_t, void),
        xprif(m_x, posx_t),
        xprif(m_y, posy_t)
    ) 
        point() : 
            point(0, 0){
        }

        point(posx_t x, posy_t y):
            m_x(x), m_y(y){
        }

        template<class finalx_t >
        point(point<finalx_t, posx_t, posy_t> const & self) :
            point(self.x(), self.y()){
        }

        final_t operator()(posx_t x, posy_t y){
            return final_t(m_x + x, m_y + y);
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
            posx_t pt = (posx_t)inc::sqrt(m_x * m_x + m_y * m_y);
            return pt;
        }
    $
}

#endif

#define xusing_math_point ::mixc::math_point
