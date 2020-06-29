#ifndef xpack_io_private_tty
#define xpack_io_private_tty
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::io_private_tty
    #include"define/base_type.hpp"
    #include"dumb/implicit.hpp"
    #include"io/private/tty_key.hpp"
    #include"io/private/tty_color_t.hpp"

    namespace mixc::io_private_tty::origin{
        extern void             print_core(asciis str, uxx length);
        extern inc::tty_key     read_key(bool echo);
        extern inc::tty_color_t backcolor();
        extern inc::tty_color_t forecolor();
        extern bool             cursor_visiable();
        extern void             cursor_visiable(bool value);
        extern void             clear();
        extern void             print_flush();
        extern void             forecolor(inc::tty_color_t value);
        extern void             backcolor(inc::tty_color_t value);
    }
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::io_private_tty::origin;
}
