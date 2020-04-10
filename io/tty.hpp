#ifndef xpack_io_tty
#define xpack_io_tty
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_tty
        #include"define/base_type.hpp"
        #include"define/platform.hpp"
        #include"io/private/tty_color_t.hpp"
        #include"io/private/printf.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_tty{
        xgc(tty_t)
            xgc_fields(
                xpro(fore               , mutable inc::tty_color_t),
                xpro(back               , mutable inc::tty_color_t),
                xpro(visiable_of_cursor , mutable bool),
            );
            using the_t = tty_t;
        private:
            auto map_forecolor(inc::tty_color_t value) const {
                static_assert(uxx(inc::black)           == 0x0);
                static_assert(uxx(inc::red)             == 0x1);
                static_assert(uxx(inc::green)           == 0x2);
                static_assert(uxx(inc::yellow)          == 0x3);
                static_assert(uxx(inc::blue)            == 0x4);
                static_assert(uxx(inc::magenta)         == 0x5);
                static_assert(uxx(inc::cyan)            == 0x6);
                static_assert(uxx(inc::white)           == 0x7);
                static_assert(uxx(inc::gray)            == 0x8);
                static_assert(uxx(inc::light_red)       == 0x9);
                static_assert(uxx(inc::light_green)     == 0xa);
                static_assert(uxx(inc::light_yellow)    == 0xb);
                static_assert(uxx(inc::light_blue)      == 0xc);
                static_assert(uxx(inc::light_magenta)   == 0xd);
                static_assert(uxx(inc::light_cyan)      == 0xe);
                static_assert(uxx(inc::light_gray)      == 0xf);

                if constexpr(xis_linux){
                    constexpr asciis map[] = {
                        "\e[30m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m", "\e[37m", "\e[90m", "\e[91m", "\e[92m", "\e[93m", "\e[94m", "\e[95m", "\e[96m", "\e[97m",
                    };
                    return map[uxx(value)];
                }
            }

            auto map_backcolor(inc::tty_color_t value) const {
                if constexpr (xis_linux){
                    constexpr asciis map[]{
                        "\e[40m", "\e[41m", "\e[42m", "\e[43m", "\e[44m", "\e[45m", "\e[46m", "\e[47m", "\e[100m", "\e[101m", "\e[102m", "\e[103m", "\e[104m", "\e[105m", "\e[106m", "\e[107m",
                    };
                    return map[uxx(value)];
                }
            }
        public:
            tty_t(){
                // backcolor(inc::black);
                forecolor(inc::light_gray);
                cursor_visiable(true);
            }

            inc::tty_color_t backcolor() const {
                return back;
            }

            bool cursor_visiable() const {
                return visiable_of_cursor;
            }

            inc::tty_color_t forecolor() const {
                return fore;
            }

            tty_t & backcolor(inc::tty_color_t * value) const {
                value[0] = back;
                return the;
            }

            tty_t & backcolor(inc::tty_color_t value) const {
                if constexpr(xis_linux){
                    inc::print(map_backcolor(back = value));
                }
                return the;
            }

            tty_t & clear() const {
                if constexpr (xis_linux){
                    inc::print("\e[0m\e[0;0H\e[2J");
                    inc::print_flush();
                    the.forecolor(fore);
                    the.backcolor(back);
                    inc::print_flush();
                }
                return the;
            }

            tty_t & cursor_visiable(bool * value) const {
                value[0] = visiable_of_cursor;
                return the;
            }

            tty_t & cursor_visiable(bool value) const {
                // need lock
                visiable_of_cursor = value;
                if constexpr(xis_linux){
                    inc::print(value ? "\e[?25h" : "\e[?25l");
                    inc::print_flush();
                }
                return the;
            }

            tty_t & forecolor(inc::tty_color_t * value) const {
                value[0] = fore;
                return the;
            }

            tty_t & forecolor(inc::tty_color_t value) const {
                if constexpr(xis_linux){
                    inc::print(map_forecolor(fore = value));
                }
                return the;
            }

            tty_t & write(asciis value) const {
                inc::print("%s", value);
                return the;
            }
        xgc_end();

        inline const tty_t tty;
    }
#endif

#include"io/private/tty_color_t.hpp"

namespace xuser::inc{
    using ::mixc::io_tty::tty;
}
