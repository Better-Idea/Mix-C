#ifndef xpack_io_private_tty_color_t
#define xpack_io_private_tty_color_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_private_tty_color_t::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_private_tty_color_t::origin{
    enum class tty_color_t : u08 {
        black,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        gray,
        light_red,
        light_green,
        light_yellow,
        light_blue,
        light_magenta,
        light_cyan,
        light_gray,
    };

    namespace tty_color{
        constexpr tty_color_t black             = tty_color_t::black;
        constexpr tty_color_t red               = tty_color_t::red;
        constexpr tty_color_t green             = tty_color_t::green;
        constexpr tty_color_t yellow            = tty_color_t::yellow;
        constexpr tty_color_t blue              = tty_color_t::blue;
        constexpr tty_color_t magenta           = tty_color_t::magenta;
        constexpr tty_color_t cyan              = tty_color_t::cyan;
        constexpr tty_color_t white             = tty_color_t::white;
        constexpr tty_color_t gray              = tty_color_t::gray;
        constexpr tty_color_t light_red         = tty_color_t::light_red;
        constexpr tty_color_t light_green       = tty_color_t::light_green;
        constexpr tty_color_t light_yellow      = tty_color_t::light_yellow;
        constexpr tty_color_t light_blue        = tty_color_t::light_blue;
        constexpr tty_color_t light_magenta     = tty_color_t::light_magenta;
        constexpr tty_color_t light_cyan        = tty_color_t::light_cyan;
        constexpr tty_color_t light_gray        = tty_color_t::light_gray;
    }
}

#endif

xexport_space(mixc::io_private_tty_color_t::origin)