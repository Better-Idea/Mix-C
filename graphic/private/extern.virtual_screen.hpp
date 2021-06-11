#ifdef xuser
#undef xuser
#endif

#define xuser   mixc::extern_gui_private_virtual_screen::inc
#include<windows.h>
#include"graphic/private/virtual_screen.hpp"

namespace mixc::extern_gui_private_virtual_screen::origin{
    void virtual_screen::configure(voidp hwnd){
        BITMAPINFOHEADER bmih{};
        bmih.biSize             = sizeof(BITMAPINFOHEADER);
        bmih.biWidth            = the.m_width;
        bmih.biHeight           = the.m_height;
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
        the.m_hdc               = GetDC(HWND(hwnd));
        the.m_mhdc              = CreateCompatibleDC(HDC(the.m_hdc));
        the.m_hbmp              = CreateDIBSection(HDC(the.m_hdc), & dbmi, DIB_RGB_COLORS, & bits, NULL, 0);
        the.m_buffer            = inc::rgbp(bits);
        SelectObject(HDC(the.m_mhdc), the.m_hbmp);
    }

    void virtual_screen::flush(){
        BitBlt(HDC(the.m_hdc), 0, 0, the.m_width, the.m_height, HDC(the.m_mhdc), 0, 0, SRCCOPY);
    }
}
