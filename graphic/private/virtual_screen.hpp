#ifndef xpack_extern_gui_private_virtual_screen
#define xpack_extern_gui_private_virtual_screen
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_private_virtual_screen::inc
#include"define/base_type.hpp"
#include"graphic/rgb.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"memop/fill.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_private_virtual_screen::origin{
    xstruct(
        xname(virtual_screen),
        xprif(m_width,       uxx),
        xprif(m_height,      uxx),
        xprif(m_buffer,      inc::rgbp),
        xprif(m_background,  inc::rgb),
        xprif(m_hdc,         voidp),
        xprif(m_mhdc,        voidp),
        xprif(m_hbmp,        voidp)
    )
        using final_t = the_t;
    public:
        xpubget_pubset(width)
        xpubget_pubset(height)
        xpubget_pubset(background)
    public:
        constexpr virtual_screen():
            m_width(),
            m_height(),
            m_buffer(),
            m_background(),
            m_hdc(),
            m_mhdc(),
            m_hbmp(){
        }

        void set_pixel(uxx x, uxx y, inc::rgb value){
            if (x >= the.m_width or y >= the.m_height){
                return;
            }
            m_buffer[y * the.m_width + x]   = value;
        }

        inc::rgb get_pixel(uxx x, uxx y){
            return m_buffer[y * the.m_width + x];
        }

        void clear(){
            inc::fill(the.m_buffer, the.m_background, the.m_height * the.m_width);
        }

        void configure(voidp hwnd);
        void flush();
    $
}

#endif

xexport_space(mixc::extern_gui_private_virtual_screen::origin)
