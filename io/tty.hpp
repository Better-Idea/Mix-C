#ifndef xpack_io_tty
#define xpack_io_tty
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_tty
        #include"configure.hpp"
        #include"define/base_type.hpp"
        #include"io/private/tty_color_t.hpp"
        #include"io/private/tty_key_t.hpp"
        #include"io/private/tty.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xprop.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_tty{
        struct tty_t{
            xgc_fields(
                xiam(tty_t),
                xpro(fore               , mutable inc::tty_color_t),
                xpro(back               , mutable inc::tty_color_t),
                xpro(visiable_of_cursor , mutable bool)
            );
        public:
            using final = tty_t;
        private:
            auto map_forecolor(inc::tty_color_t value) const {
                using namespace inc::tty_color;
                static_assert(uxx(black)           == 0x0);
                static_assert(uxx(red)             == 0x1);
                static_assert(uxx(green)           == 0x2);
                static_assert(uxx(yellow)          == 0x3);
                static_assert(uxx(blue)            == 0x4);
                static_assert(uxx(magenta)         == 0x5);
                static_assert(uxx(cyan)            == 0x6);
                static_assert(uxx(white)           == 0x7);
                static_assert(uxx(gray)            == 0x8);
                static_assert(uxx(light_red)       == 0x9);
                static_assert(uxx(light_green)     == 0xa);
                static_assert(uxx(light_yellow)    == 0xb);
                static_assert(uxx(light_blue)      == 0xc);
                static_assert(uxx(light_magenta)   == 0xd);
                static_assert(uxx(light_cyan)      == 0xe);
                static_assert(uxx(light_gray)      == 0xf);

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
                // backcolor(inc::tty_color::black);
                forecolor(inc::tty_color::light_gray);
                cursor_visiable(true);
            }

            xpubget_pubset(backcolor, inc::tty_color_t){
                xr { return the.back; }
                xw {
                    if constexpr(xis_linux){
                        the.back = value;
                        inc::print(the.map_backcolor(value));
                    }
                }
            };

            xpubget_pubset(cursor_visiable, bool){
                xr { return the.visiable_of_cursor; }
                xw {
                    if constexpr(xis_linux){
                        the.visiable_of_cursor = value;
                        inc::print(value ? "\e[?25h" : "\e[?25l");
                        inc::print_flush();
                    }
                }
            };

            xpubget_pubset(forecolor, inc::tty_color_t){
                xr { return the.back; }
                xw {
                    if constexpr(xis_linux){
                        the.fore = value;
                        inc::print(the.map_forecolor(value));
                    }
                }
            };

            xpubget(read_key, inc::tty_key_t){
                return inc::read_key();
            };

            final & clear() const {
                if constexpr(xis_linux){
                    inc::print("\e[0m\e[0;0H\e[2J");
                    // the.backcolor(back);
                    the.forecolor(fore);
                    the.flush();
                }
                return thex;
            }

            final & flush() const {
                inc::print_flush();
                return thex;
            }
            
            template<class a0, class ... args>
            final & write(a0 const & first, args const & ... list) const {
                inc::print(first, list...);
                return thex;
            }
            
            template<class a0, class ... args>
            final & write_line(a0 const & first, args const & ... list) const {
                inc::print(first, list..., '\n');
                return thex;
            }
        };

        inline tty_t tty;
    }
#endif

namespace xuser::inc{
    using ::mixc::io_tty::tty;
}

#include"io/private/tty_color_t.hpp"
