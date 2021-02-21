#ifdef xuser
#undef xuser
#endif

#define xuser   mixc::extern_gui_private_virtual_screen::inc
#include<windows.h>
#include"extern/gui/private/virtual_screen.hpp"

namespace mixc::extern_gui_private_virtual_screen::origin{
    void virtual_screen::configure(voidp hwnd){
        BITMAPINFOHEADER bmih{};
        bmih.biSize             = sizeof(BITMAPINFOHEADER);
        bmih.biWidth            = the.pwidth;
        bmih.biHeight           = the.pheight;
        bmih.biPlanes           = 1;
        bmih.biBitCount         = 24;
        bmih.biCompression      = BI_RGB;
        bmih.biSizeImage        = 0;
        bmih.biXPelsPerMeter    = 0;
        bmih.biYPelsPerMeter    = 0;
        bmih.biClrUsed          = 0;
        bmih.biClrImportant     = 0;

        BITMAPINFO dbmi{};
        dbmi.bmiHeader          = bmih;

        voidp   bits            = nullptr;
        the.phdc                = GetDC(HWND(hwnd));
        the.pmhdc               = CreateCompatibleDC(HDC(the.phdc));
        the.phbmp               = CreateDIBSection(HDC(the.phdc), & dbmi, DIB_RGB_COLORS, & bits, NULL, 0);
        the.pbuffer             = inc::rgbp(bits);
        SelectObject(HDC(the.pmhdc), the.phbmp);
    }

    void virtual_screen::flush(){
        BitBlt(HDC(the.phdc), 0, 0, the.pwidth, the.pheight, HDC(the.pmhdc), 0, 0, SRCCOPY);
    }
}
