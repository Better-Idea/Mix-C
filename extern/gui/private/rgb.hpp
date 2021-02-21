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
        xprif(pblue  , u08),
        xprif(pgreen , u08),
        xprif(pred   , u08)
    )
        template<class finalx_t>
        requires(sizeof(finalx_t) == sizeof(final_t))
        constexpr rgb(rgb<finalx_t> const & self){
            thex = (final_t &)self;
        }

        constexpr rgb(u08 r, u08 g, u08 b) : 
            pblue(b), pgreen(g), pred(r) {
        }

        constexpr rgb() : 
            pblue(0), pgreen(0), pred(0){
        }

        xpubget_pubset(red)
        xpubget_pubset(green)
        xpubget_pubset(blue)

        xpubgetx(value, u32){
            return u32(pred) | u32(pgreen) << 8 | u32(pblue) << 16;
        }
    $
}

#endif

#define xusing_extern_gui_rgb    ::mixc::extern_gui_rgb::origin
