#ifndef xpack_extern_gui_private_virtual_screen
#define xpack_extern_gui_private_virtual_screen
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_private_virtual_screen::inc
#include"graphic/rgb.hpp"
#include"memop/fill.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_private_virtual_screen::origin{
    xstruct(
        xname(virtual_screen),
        xprif(pwidth,       uxx),
        xprif(pheight,      uxx),
        xprif(pbuffer,      inc::rgbp),
        xprif(pbackground,  inc::rgb),
        xprif(phdc,         voidp),
        xprif(pmhdc,        voidp),
        xprif(phbmp,        voidp)
    )
        using final_t = the_t;
    public:
        xpubget_pubset(width)
        xpubget_pubset(height)
        xpubget_pubset(background)
    public:
        constexpr virtual_screen():
            pwidth(),
            pheight(),
            pbuffer(),
            pbackground(),
            phdc(),
            pmhdc(),
            phbmp(){
        }

        void set_pixel(uxx x, uxx y, inc::rgb value){
            if (x >= the.pwidth or y >= the.pheight){
                return;
            }
            pbuffer[y * the.pwidth + x]   = value;
        }

        inc::rgb get_pixel(uxx x, uxx y){
            return pbuffer[y * the.pwidth + x];
        }

        void clear(){
            inc::fill(the.pbuffer, the.pbackground, the.pheight * the.pwidth);
        }

        void configure(voidp hwnd);
        void flush();
    $
}

#endif

xexport_space(mixc::extern_gui_private_virtual_screen::origin)
