/*
问题：
- ctrl+m 会被映射成 ctrl+j
- 部分 ctrl+字母 和 ascii 的转义符重合，比如 \a \b \t \v \n

查表:
l          : 1b,5b,44,
shift+l    : 1b,5b,31,3b,32,44,
ALT+l      : 1b,5b,31,3b,33,44,
ctr+l      : 1b,5b,31,3b,35,44,
CS+l       : 1b,5b,31,3b,36,44,
CA+l       : 1b,5b,31,3b,37,44,
t          : 1b,5b,41,
shift+t    : 1b,5b,31,3b,32,41,
ALT+t      : 1b,5b,31,3b,33,41,
ctr+t      : 1b,5b,31,3b,35,41,
CS+t       : 1b,5b,31,3b,36,41,
CA+t       : 1b,5b,31,3b,37,41,
r          : 1b,5b,43,
shift+r    : 1b,5b,31,3b,32,43,
ALT+r      : 1b,5b,31,3b,33,43,
ctr+r      : 1b,5b,31,3b,35,43,
CS+r       : 1b,5b,31,3b,36,43,
CA+r       : 1b,5b,31,3b,37,43,
b          : 1b,5b,42,
shift+b    : 1b,5b,31,3b,32,42,
ALT+b      : 1b,5b,31,3b,33,42,
ctr+b      : 1b,5b,31,3b,35,42,
CS+b       : 1b,5b,31,3b,36,42,
CA+b       : 1b,5b,31,3b,37,42,

del
1b,5b,33,7e,
1b,5b,33,3b,32,7e,
1b,5b,33,3b,33,7e,
1b,5b,33,3b,35,7e,

ins
1b,5b,32,7e,
1b,5b,32,3b,35,7e,
1b,5b,32,3b,32,7e,
1b,5b,32,3b,33,7e,

Home       : 1b,5b,48,
PgUp       : 1b,5b,35,7e,
End        : 1b,5b,46,
PgDn       : 1b,5b,36,7e,


Home
PgUp
End
PgDn

ALT:Home    : 1b,5b,31,3b,33,48,
ALT:PgUp    : 1b,5b,35,3b,33,7e,
ALT:End     : 1b,5b,31,3b,33,46,
ALT:PgDn    : 1b,5b,36,3b,33,7e,

F1         : 1b,4f,50,
SHIFT+F1   : 1b,5b,31,3b,32,50,
ALT+F1     : 1b,5b,31,3b,33,50,
AS+F1      : 1b,5b,31,3b,34,50,
CTRL+F1    : 1b,5b,31,3b,35,50,
CS+F1      : 1b,5b,31,3b,36,50,
CA+F1      : 1b,5b,31,3b,37,50,
ASC+F1     : 1b,5b,31,3b,38,50,
F2         : 1b,4f,51,
SHIFT+F2   : 1b,5b,31,3b,32,51,
ALT+F2     : 1b,5b,31,3b,33,51,
AS+F2      : 1b,5b,31,3b,34,51,
CTRL+F2    : 1b,5b,31,3b,35,51,
CS+F2      : 1b,5b,31,3b,36,51,
CA+F2      : 1b,5b,31,3b,37,51,
ASC+F2     : 1b,5b,31,3b,38,51,
F3         : 1b,4f,52,
SHIFT+F3   : 1b,5b,31,3b,32,52,
ALT+F3     : 1b,5b,31,3b,33,52,
AS+F3      : 1b,5b,31,3b,34,52,
CTRL+F3    : 1b,5b,31,3b,35,52,
CS+F3      : 1b,5b,31,3b,36,52,
CA+F3      : 1b,5b,31,3b,37,52,
ASC+F3     : 1b,5b,31,3b,38,52,
F4         : 1b,4f,53,
SHIFT+F4   : 1b,5b,31,3b,32,53,
ALT+F4     : 1b,5b,31,3b,33,53,
AS+F4      : 1b,5b,31,3b,34,53,
CTRL+F4    : 1b,5b,31,3b,35,53,
CS+F4      : 1b,5b,31,3b,36,53,
CA+F4      : 1b,5b,31,3b,37,53,
ASC+F4     : 1b,5b,31,3b,38,53,
F5         : 1b,5b,31,35,7e,
SHIFT+F5   : 1b,5b,31,35,3b,32,7e,
ALT+F5     : 1b,5b,31,35,3b,33,7e,
AS+F5      : 1b,5b,31,35,3b,34,7e,
CTRL+F5    : 1b,5b,31,35,3b,35,7e,
CS+F5      : 1b,5b,31,35,3b,36,7e,
CA+F5      : 1b,5b,31,35,3b,37,7e,
ASC+F5     : 1b,5b,31,35,3b,38,7e,
F6         : 1b,5b,31,37,7e,
SHIFT+F6   : 1b,5b,31,37,3b,32,7e,
ALT+F6     : 1b,5b,31,37,3b,33,7e,
AS+F6      : 1b,5b,31,37,3b,34,7e,
CTRL+F6    : 1b,5b,31,37,3b,35,7e,
CS+F6      : 1b,5b,31,37,3b,36,7e,
CA+F6      : 1b,5b,31,37,3b,37,7e,
ASC+F6     : 1b,5b,31,37,3b,38,7e,
F7         : 1b,5b,31,38,7e,
SHIFT+F7   : 1b,5b,31,38,3b,32,7e,
ALT+F7     : 1b,5b,31,38,3b,33,7e,
AS+F7      : 1b,5b,31,38,3b,34,7e,
CTRL+F7    : 1b,5b,31,38,3b,35,7e,
CS+F7      : 1b,5b,31,38,3b,36,7e,
CA+F7      : 1b,5b,31,38,3b,37,7e,
ASC+F7     : 1b,5b,31,38,3b,38,7e,
F8         : 1b,5b,31,39,7e,
SHIFT+F8   : 1b,5b,31,39,3b,32,7e,
ALT+F8     : 1b,5b,31,39,3b,33,7e,
AS+F8      : 1b,5b,31,39,3b,34,7e,
CTRL+F8    : 1b,5b,31,39,3b,35,7e,
CS+F8      : 1b,5b,31,39,3b,36,7e,
CA+F8      : 1b,5b,31,39,3b,37,7e,
ASC+F8     : 1b,5b,31,39,3b,38,7e,
F9         : 1b,5b,32,30,7e,
SHIFT+F9   : 1b,5b,32,30,3b,32,7e,
ALT+F9     : 1b,5b,32,30,3b,33,7e,
AS+F9      : 1b,5b,32,30,3b,34,7e,
CTRL+F9    : 1b,5b,32,30,3b,35,7e,
CS+F9      : 1b,5b,32,30,3b,36,7e,
CA+F9      : 1b,5b,32,30,3b,37,7e,
ASC+F9     : 1b,5b,32,30,3b,38,7e,
F10        : 1b,5b,32,31,7e,
SHIFT+F10  : 1b,5b,32,31,3b,32,7e,
ALT+F10    : 1b,5b,32,31,3b,33,7e,
AS+F10     : 1b,5b,32,31,3b,34,7e,
CTRL+F10   : 1b,5b,32,31,3b,35,7e,
CS+F10     : 1b,5b,32,31,3b,36,7e,
CA+F10     : 1b,5b,32,31,3b,37,7e,
ASC+F10    : 1b,5b,32,31,3b,38,7e,
F11        : 1b,5b,32,33,7e,
SHIFT+F11  : 1b,5b,32,33,3b,32,7e,
ALT+F11    : 1b,5b,32,33,3b,33,7e,
AS+F11     : 1b,5b,32,33,3b,34,7e,
CTRL+F11   : 1b,5b,32,33,3b,35,7e,
CS+F11     : 1b,5b,32,33,3b,36,7e,
CA+F11     : 1b,5b,32,33,3b,37,7e,
ASC+F11    : 1b,5b,32,33,3b,38,7e,
F12        : 1b,5b,32,34,7e,
SHIFT+F12  : 1b,5b,32,34,3b,32,7e,
ALT+F12    : 1b,5b,32,34,3b,33,7e,
AS+F12     : 1b,5b,32,34,3b,34,7e,
CTRL+F12   : 1b,5b,32,34,3b,35,7e,
CS+F12     : 1b,5b,32,34,3b,36,7e,
CA+F12     : 1b,5b,32,34,3b,37,7e,
ASC+F12    : 1b,5b,32,34,3b,38,7e,

*/

#define xuser mixc::io_private_tty
#include<stdio.h>
#include"algo/binary_search.hpp"
#include"configure.hpp"
#include"docker/array.hpp"
#include"io/private/tty.hpp"
#include"io/private/tty_key_t.hpp"
#include"io/private/tty_color_t.hpp"
#include"lang/cxx/compare.hpp"
#include"lang/cxx.hpp"
#include"macro/xdebug_fail.hpp"

#if xis_windows
    #include<windows.h>
#endif

namespace mixc::io_private_tty{
    print_t const        print_core = & printf;
    inc::tty_key_t const unknown_key{};

    struct{
        u08 fore : 4;
        u08 back : 4;
        operator u08(){
            return *u08p(this);
        }
    } color;
    
    bool the_cursor_visiable = true;
    
    inc::tty_color_t backcolor() {
        return inc::tty_color_t(color.back);
    }

    inc::tty_color_t forecolor() {
        return inc::tty_color_t(color.fore);
    }

    bool cursor_visiable(){
        return the_cursor_visiable;
    }

    void print_flush(){
        fflush(stdout);
    }

    inc::tty_key_t decode(inc::c16 codes, uxx * rest){
        using namespace inc::func_key;

        auto    key = inc::tty_key_t();
        auto && ascii = [](char code, inc::tty_key_t key){
            switch(code) {
            case 0x1b: return key.is_func(true).value(esc);
            case 0x7f: return key.is_func(true).value(backspace);
            case '\r': // conflict =========================
            case '\n': return key.is_func(true).value(enter);
            }
            if (key.is_char(true); ' ' <= code and code <= '~'){ // is ascii
                return key.value(code);
            }
            if (key.has_ctrl(true); code <= 0x1a){
                return key.value('a' + code - 1);
            }
            switch(code){
            // case 0x1b: return key.value('['); ========================= conflict
            case 0x1c: return key.value('\\');
            case 0x1d: return key.value(']');
            case 0x1f: return key.value('/');
            }
            return unknown_key;
        };

        auto && func = [](char code, inc::tty_key_t key){
            switch(key.is_func(true); code){
            case 0x41: return key.value(top);
            case 0x42: return key.value(buttom);
            case 0x43: return key.value(right);
            case 0x44: return key.value(left);
            case 0x50: return key.value(f1);
            case 0x51: return key.value(f2);
            case 0x52: return key.value(f3);
            case 0x53: return key.value(f4);
            }
            return unknown_key;
        };

        auto && funcii = [](char code, inc::tty_key_t key){
            switch(key.is_func(true); code){
            case 0x35: return key.value(f5);
            case 0x37: return key.value(f6);
            case 0x38: return key.value(f7);
            case 0x39: return key.value(f8);
            case 0x30: return key.value(f9);
            case 0x31: return key.value(f10);
            case 0x33: return key.value(f11);
            case 0x34: return key.value(f12);
            }
            return unknown_key;
        };

        auto && funciii = [](char code, inc::tty_key_t key){
            return key.is_func(true).value(
                code == 0x32 ? ins :
                code == 0x33 ? del : 0 // 0 indicated the unknown key
            );
        };

        if (u16(codes[0]) > 0x7f or (' ' <= codes[0] and codes[0] <= '~')){ // normal char
            rest[0] = codes.length() - 1;
            return key.is_char(true).value(codes[0]);
        }
        if (rest[0] = 0; codes.length() == 1){
            return ascii(codes[0], key);
        }

        enum{
            has_shift   = 0x1,
            has_ctrl    = 0x2,
            has_alt     = 0x4,
        };

        constexpr u08 modify_lut[] = {
            has_shift,
            has_alt,
            has_shift | has_alt,
            has_ctrl,
            has_ctrl | has_shift,
            has_ctrl | has_alt,
            has_ctrl | has_shift | has_alt,
        };

        u08 modify = 0;

        switch(codes.length()){
        case 2:
            return ascii(codes[1], key.has_alt(true));
        case 3:
            return func(codes[2], key);
        case 4: 
            return funciii(codes[2], key);
        case 5:
            return funcii(codes[3], key);
        case 6: 
            if (modify = modify_lut[codes[4] - 0x32]; codes[5] == 0x7e){
                key = funciii(codes[2], key);
            }
            else{
                key = func(codes[5], key);
            }
            break;
        case 7:
            modify = modify_lut[codes[5] - 0x32];
            key = funcii(codes[3], key);
            break;
        }

        return key
            .has_shift(modify & has_shift)
            .has_alt(modify & has_alt)
            .has_ctrl(modify & has_ctrl);
    }

    #if xis_linux
    #include<fcntl.h>
    #include<termios.h>
    #include<unistd.h>
    using namespace inc::func_key;
    using namespace io_private;

    void forecolor(inc::tty_color_t value) {
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

        constexpr asciis map[] = {
            "\e[30m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m", "\e[37m", "\e[90m", "\e[91m", "\e[92m", "\e[93m", "\e[94m", "\e[95m", "\e[96m", "\e[97m",
        };
        print_core(map[uxx(index)]);
    }

    void backcolor(inc::tty_color_t value) {
        constexpr asciis map[]{
            "\e[40m", "\e[41m", "\e[42m", "\e[43m", "\e[44m", "\e[45m", "\e[46m", "\e[47m", "\e[100m", "\e[101m", "\e[102m", "\e[103m", "\e[104m", "\e[105m", "\e[106m", "\e[107m",
        };
        print_core(map[uxx(index)]);
    }

    void clear(){
        print_core("\e[0m\e[0;0H\e[2J");
        // backcolor(back);
        forecolor(color.fore);
        flush();
    }

    void cursor_visiable(bool value){
        cursor_visiable = true;
        print_core(value ? "\e[?25h" : "\e[?25l");
        print_flush();
    }

    struct ini{
        ini(){
            termios oldattr;
            tcgetattr(STDIN_FILENO, & oldattr);
            oldattr.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
            oldattr.c_cc[VSTART] = 0xff;
            oldattr.c_cc[VSTOP]  = 0xff;
            tcsetattr(STDIN_FILENO, TCSANOW, & oldattr);
        }
    } __ini;

    uxx hit(){
        auto oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        uxx v = uxx(getchar());
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        return v;
    }

    constexpr uxx buf_size = 16;
    u08           buf[buf_size + 1];
    uxx           rest = 0;

    inc::tty_key_t read_key(){
        // u08p          end = buf + buf_size;
        // u08p          ptr = buf;
        // ptr[0]            = getchar(); // 阻塞读取
        // ptr              += 1;

        // while(ptr < end){
        //    if (auto v = hit(); v == not_exist){ // 非阻塞读取
        //        break;
        //    }
        //    else{
        //        ptr[0] = u08(v);
        //        ptr   += 1;
        //    }
        // }
        // ptr[0] = '\0'; // end

        // return decode(inc::c08{ buf, ptr - buf }, & rest);
    }
    #elif xis_windows

    char16_t buf_key[8];
    uxx      rest;
    inc::c16 key_str;

    inc::tty_key_t read_key(bool echo){
        // need lock ===================================
        inc::tty_key_t key;

        if (key_str.length() == 0) {
            HANDLE         h = GetStdHandle(STD_INPUT_HANDLE);
            DWORD          mode;
            DWORD          length;
            GetConsoleMode(h, & mode);
            SetConsoleMode(h, ~(
                ENABLE_LINE_INPUT | 
                ENABLE_PROCESSED_INPUT | (
                    not echo ? ENABLE_ECHO_INPUT : 0
                )
            ));
            ReadConsoleW(h, buf_key, sizeof(buf_key) / sizeof(buf_key[0]), & length, NULL);
            SetConsoleMode(h, mode); // recover
            key_str = { buf_key, length };
        }

        key     = decode(key_str, & rest);
        key_str = key_str.backward(key_str.length() - rest);

        if (auto v = key.value(); key.is_char()) {
            WideCharToMultiByte(CP_ACP, 0, LPCWCH(& v), 1, (LPSTR)key.multi_bytes_char(), sizeof(inc::tty_key_t::items_t), NULL, NULL);
        }
        return key;
    }

    constexpr u08 map[]{
        0 << 3 | 0 << 2 | 0 << 1 | 0 << 0,
        0 << 3 | 1 << 2 | 0 << 1 | 0 << 0,
        0 << 3 | 0 << 2 | 1 << 1 | 0 << 0,
        0 << 3 | 1 << 2 | 1 << 1 | 0 << 0,
        0 << 3 | 0 << 2 | 0 << 1 | 1 << 0,
        0 << 3 | 0 << 2 | 1 << 1 | 1 << 0,
        0 << 3 | 1 << 2 | 0 << 1 | 1 << 0,
        1 << 3 | 1 << 2 | 1 << 1 | 1 << 0,
        1 << 3 | 0 << 2 | 0 << 1 | 0 << 0,
        1 << 3 | 1 << 2 | 0 << 1 | 0 << 0,
        1 << 3 | 0 << 2 | 1 << 1 | 0 << 0,
        1 << 3 | 1 << 2 | 1 << 1 | 0 << 0,
        1 << 3 | 0 << 2 | 0 << 1 | 1 << 0,
        1 << 3 | 0 << 2 | 1 << 1 | 1 << 0,
        1 << 3 | 1 << 2 | 0 << 1 | 1 << 0,
        0 << 3 | 1 << 2 | 1 << 1 | 1 << 0,
    };

    void forecolor(inc::tty_color_t value) {
        color.fore = map[uxx(value)];
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void backcolor(inc::tty_color_t value) {
        color.back = map[uxx(value)];
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void clear(){
        backcolor(inc::tty_color_t(color.back));
        forecolor(inc::tty_color_t(color.fore));
        print_flush();
    }

    void cursor_visiable(bool value){
        CONSOLE_CURSOR_INFO info;    
        info.bVisible = value;    
        info.dwSize = sizeof(info);
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), & info);
    }

    #endif
}
