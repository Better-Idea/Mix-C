#ifndef xpack_io_private_tty_color_t
#define xpack_io_private_tty_color_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_private_tty_color_t
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_private_tty_color_t{
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

#endif

namespace xuser::inc{
    using ::mixc::io_private_tty_color_t::tty_color_t;
    using ::mixc::io_private_tty_color_t::black;
    using ::mixc::io_private_tty_color_t::red;
    using ::mixc::io_private_tty_color_t::green;
    using ::mixc::io_private_tty_color_t::yellow;
    using ::mixc::io_private_tty_color_t::blue;
    using ::mixc::io_private_tty_color_t::magenta;
    using ::mixc::io_private_tty_color_t::cyan;
    using ::mixc::io_private_tty_color_t::white;
    using ::mixc::io_private_tty_color_t::gray;
    using ::mixc::io_private_tty_color_t::light_red;
    using ::mixc::io_private_tty_color_t::light_green;
    using ::mixc::io_private_tty_color_t::light_yellow;
    using ::mixc::io_private_tty_color_t::light_blue;
    using ::mixc::io_private_tty_color_t::light_magenta;
    using ::mixc::io_private_tty_color_t::light_cyan;
    using ::mixc::io_private_tty_color_t::light_gray;
}
