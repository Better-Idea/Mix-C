#ifdef  xuser
#undef  xuser
#endif

#define xuser  mixc::powerful_cat::inc
#include<windows.h>
#include"graphic/private/virtual_screen.hpp"
#include"graphic/private/extern.virtual_screen.hpp"

xuser::virtual_screen vscreen;

void vdisplay(xuser::virtual_screen & vscreen);

LRESULT CALLBACK event(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch(message){
    case WM_LBUTTONUP:
        vdisplay(vscreen);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(
    HINSTANCE   instance,
    HINSTANCE   prev_instance,
    PSTR        cmd_line,
    int         state){

    HWND        hwnd;
    MSG         msg;
    WNDCLASS    wndclass{};
    auto        class_name  = TEXT("Mix-C");
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = event;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = instance;
    wndclass.hIcon          = LoadIcon(nullptr, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hbrBackground  = HBRUSH(GetStockObject(BLACK_BRUSH));
    wndclass.lpszMenuName   = nullptr;
    wndclass.lpszClassName  = class_name;

    if (not RegisterClass(& wndclass)){
        MessageBox(nullptr, TEXT("This program requires Windows NT!"), class_name, MB_ICONERROR);
        return 0;
    }

    auto width  = (600);
    auto height = (800);
    auto x      = (GetSystemMetrics(SM_CXSCREEN) - width ) / 2;
    auto y      = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    hwnd        = CreateWindow(
        class_name,             // window class name
        nullptr,                // window caption
        WS_VISIBLE | WS_POPUP,  // window style
        x,                      // initial x position
        y,                      // initial y position
        width,                  // initial x size
        height,                 // initial y size
        nullptr,                // parent window handle
        nullptr,                // window menu handle
        instance,               // program instance handle
        nullptr);               // creation parameters

    vscreen.width(width).height(height);
    vscreen.configure(hwnd);
    vscreen.clear();

    ShowWindow(hwnd, state);
    UpdateWindow(hwnd);

    while(GetMessage(& msg, nullptr, 0, 0)){
        TranslateMessage(& msg);
        DispatchMessage(& msg);
    }
    return msg.wParam;
}
