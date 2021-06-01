#ifndef xpack_extern_gui_rgb
#define xpack_extern_gui_rgb
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_rgb::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_rgb::origin {
    template<class final_t>
    xstruct(
        xtmpl(rgb, final_t),
        xprif(m_blue  , u08),
        xprif(m_green , u08),
        xprif(m_red   , u08)
    )
        template<class finalx_t>
        constexpr rgb(rgb<finalx_t> const & self){
            thex = (final_t &)self;
            static_assert(sizeof(finalx_t) == sizeof(final_t));
        }

        constexpr rgb(u08 r, u08 g, u08 b) : 
            m_blue(b), m_green(g), m_red(r) {
        }

        constexpr rgb() : 
            m_blue(0), m_green(0), m_red(0){
        }

        xpubget_pubset(red)
        xpubget_pubset(green)
        xpubget_pubset(blue)

        xpubgetx(value, u32){
            return u32(m_red) | u32(m_green) << 8 | u32(m_blue) << 16;
        }
    $
}

#endif

#define xusing_extern_gui_rgb    ::mixc::extern_gui_rgb::origin
