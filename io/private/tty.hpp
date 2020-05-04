#ifndef xpack_io_private_tty
#define xpack_io_private_tty
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_private_tty
        #include"define/base_type.hpp"
        #include"dumb/implicit.hpp"
        #include"io/private/tty_key_t.hpp"
        #include"io/private/tty_color_t.hpp"
        #include"macro/private/mix.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_private_tty{
        using print_t = int (*)(const char * fmt,...);
        extern print_t const    print_core;
        extern inc::tty_key_t   read_key(bool echo);
        extern inc::tty_color_t backcolor();
        extern inc::tty_color_t forecolor();
        extern bool             cursor_visiable();
        extern void             cursor_visiable(bool value);
        extern void             clear();
        extern void             print_flush();
        extern void             forecolor(inc::tty_color_t value);
        extern void             backcolor(inc::tty_color_t value);

        template<class ... args>
        void print(args const & ... list){
            inc::implicit<inc::mix> items[] = { list... };
            for(auto item : items){
                print_core(item->fmt, item->v);
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::io_private_tty::backcolor;
    using ::mixc::io_private_tty::clear;
    using ::mixc::io_private_tty::cursor_visiable;
    using ::mixc::io_private_tty::forecolor;
    using ::mixc::io_private_tty::print;
    using ::mixc::io_private_tty::print_core;
    using ::mixc::io_private_tty::print_flush;
    using ::mixc::io_private_tty::read_key;
}
