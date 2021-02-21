#ifdef  xuser
#ifndef xusing_extern_gui_rgb
#include"graphic/private/rgb.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser{
    xfinal(xusing_extern_gui_rgb, rgb);

    using rgbp = rgb *;
}

#undef  xusing_extern_gui_rgb
#endif
