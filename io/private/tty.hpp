#ifndef xpack_io_private_tty
#define xpack_io_private_tty
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_private_tty::inc
#include"interface/can_alloc.hpp"
#include"io/private/tty_key.hpp"
#include"io/private/tty_color_t.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_private_tty::origin{
    extern void             print_core(asciis str, uxx length);
    extern inc::tty_key     read_key(bool echo);
    extern inc::c08         read_line(inc::ialloc<char> alloc);
    extern inc::c16         read_line(inc::ialloc<char16_t> alloc);
    extern inc::tty_color_t backcolor();
    extern inc::tty_color_t forecolor();
    extern bool             cursor_visiable();
    extern void             cursor_visiable(bool value);
    extern void             clear();
    extern void             print_flush();
    extern void             forecolor(inc::tty_color_t value);
    extern void             backcolor(inc::tty_color_t value);
}
#endif

xexport_space(mixc::io_private_tty::origin)