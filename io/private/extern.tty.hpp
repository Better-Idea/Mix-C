/*
问题：
- ctrl+m 会被映射成 ctrl+j
- 部分 ctrl+字母 和 ascii 的转义符重合，比如 \a \b \t \v \n

查表:
p0 <=> 1b 5b
p1 <=> 1b 5b 31
p2 <=> 1b 5b 31 3b
p3 <=> 1b 5b 32

        top         buttom      right       left        delete            insert            end         home        pgup              pgdn             
none    p0 41       p0 42       p0 43       p0 44       p0 33 7e          p0 32 7e          p0 46       p0 48       p0 35             p0 36            
shift   p2 32 41    p2 32 42    p2 32 43    p2 32 44    p0 33 3b 32 44    p0 32 3b 32 7e    p2 32 46    p2 32 48    p0 35 3b 32 7e    p0 36 3b 32 7e   
alt     p2 33 41    p2 33 42    p2 33 43    p2 33 44    p0 33 3b 33 44    p0 32 3b 33 7e    p2 33 46    p2 33 48    p0 35 3b 33 7e    p0 36 3b 33 7e   
s+a     p2 34 41    p2 34 42    p2 34 43    p2 34 44    p0 33 3b 34 44    p0 32 3b 34 7e    p2 34 46    p2 34 48    p0 35 3b 34 7e    p0 36 3b 34 7e   
ctrl    p2 35 41    p2 35 42    p2 35 43    p2 35 44    p0 33 3b 35 44    p0 32 3b 35 7e    p2 35 46    p2 35 48    p0 35 3b 35 7e    p0 36 3b 35 7e   
c+s     p2 36 41    p2 36 42    p2 36 43    p2 36 44    p0 33 3b 36 44    p0 32 3b 36 7e    p2 36 46    p2 36 48    p0 35 3b 36 7e    p0 36 3b 36 7e   
c+a     p2 37 41    p2 37 42    p2 37 43    p2 37 44    p0 33 3b 37 44    p0 32 3b 37 7e    p2 37 46    p2 37 48    p0 35 3b 37 7e    p0 36 3b 37 7e   
c+s+a   p2 38 41    p2 38 42    p2 38 43    p2 38 44    p0 33 3b 38 44    p0 32 3b 38 7e    p2 38 46    p2 38 48    p0 35 3b 38 7e    p0 36 3b 38 7e   


        f1          f2          f3          f4          f5               f6               f7               f8               f9               f10              f11              f12               
none    1b 4f 50    1b 4f 51    1b 4f 52    1b 4f 53    p1 35 7e         p1 37 7e         p1 38 7e         p1 39 7e         p3 30 7e         p3 31 7e         p3 33 7e         p3 34 7e         
shift   p2 32 50    p2 32 51    p2 32 52    p2 32 53    p1 35 3b 32 7e   p1 37 3b 32 7e   p1 38 3b 32 7e   p1 39 3b 32 7e   p3 30 3b 32 7e   p3 31 3b 32 7e   p3 33 3b 32 7e   p3 34 3b 32 7e   
alt     p2 33 50    p2 33 51    p2 33 52    p2 33 53    p1 35 3b 33 7e   p1 37 3b 33 7e   p1 38 3b 33 7e   p1 39 3b 33 7e   p3 30 3b 33 7e   p3 31 3b 33 7e   p3 33 3b 33 7e   p3 34 3b 33 7e   
s+a     p2 34 50    p2 34 51    p2 34 52    p2 34 53    p1 35 3b 34 7e   p1 37 3b 34 7e   p1 38 3b 34 7e   p1 39 3b 34 7e   p3 30 3b 34 7e   p3 31 3b 34 7e   p3 33 3b 34 7e   p3 34 3b 34 7e   
ctrl    p2 35 50    p2 35 51    p2 35 52    p2 35 53    p1 35 3b 35 7e   p1 37 3b 35 7e   p1 38 3b 35 7e   p1 39 3b 35 7e   p3 30 3b 35 7e   p3 31 3b 35 7e   p3 33 3b 35 7e   p3 34 3b 35 7e   
c+s     p2 36 50    p2 36 51    p2 36 52    p2 36 53    p1 35 3b 36 7e   p1 37 3b 36 7e   p1 38 3b 36 7e   p1 39 3b 36 7e   p3 30 3b 36 7e   p3 31 3b 36 7e   p3 33 3b 36 7e   p3 34 3b 36 7e   
c+a     p2 37 50    p2 37 51    p2 37 52    p2 37 53    p1 35 3b 37 7e   p1 37 3b 37 7e   p1 38 3b 37 7e   p1 39 3b 37 7e   p3 30 3b 37 7e   p3 31 3b 37 7e   p3 33 3b 37 7e   p3 34 3b 37 7e   
c+s+a   p2 38 50    p2 38 51    p2 38 52    p2 38 53    p1 35 3b 38 7e   p1 37 3b 38 7e   p1 38 3b 38 7e   p1 39 3b 38 7e   p3 30 3b 38 7e   p3 31 3b 38 7e   p3 33 3b 38 7e   p3 34 3b 38 7e   
*/

#undef  xuser
#define xuser mixc::io_private_tty::inc
#include<stdio.h>
#include"configure/switch.hpp"
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"io/private/tty.hpp"
#include"io/private/tty_color_t.hpp"
#include"io/private/tty_key.hpp"
#include"macro/xdebug_fail.hpp"
#include"macro/xinit.hpp"
#include"macro/xref.hpp"
#include"memop/copy.hpp"
#include"utils/memory.hpp"

#if xis_windows
#include<windows.h>
#endif

#if xis_linux
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<wchar.h>
#endif

namespace mixc::io_private_tty::origin{
    union color_t{
        struct{
            u08 fore           : 4;
            u08 back           : 4;
        };

        operator u08(){
            return *u08p(this);
        }
    };

    inline inc::tty_key const   unknown_key{};
    inline inc::c16             key_str;
    inline char16_t             buf_key[8];
    inline uxx                  rest;
    inline bool                 the_cursor_visiable = true;
    inline color_t              color;
    inline uxx                  cursor_width;

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

    inc::tty_key decode(inc::c16 codes, uxx * rest){
        using namespace inc::func_key;

        auto    key = inc::tty_key();
        auto && ascii = [](auto code, auto key){
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

        auto && func = [](auto code, auto key){
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

        auto && funcii = [](auto code, auto key){
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

        auto && funciii = [](auto code, auto key){
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
    using namespace inc::func_key;

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
        print_core(map[uxx(value)], 5/*length*/);
    }

    void backcolor(inc::tty_color_t value) {
        constexpr asciis map[]{
            "\e[40m", "\e[41m", "\e[42m", "\e[43m", "\e[44m", "\e[45m", "\e[46m", "\e[47m", "\e[100m", "\e[101m", "\e[102m", "\e[103m", "\e[104m", "\e[105m", "\e[106m", "\e[107m",
        };
        print_core(map[uxx(value)], 5/*length*/);
    }

    void clear(){
        print_core("\e[0m\e[0;0H\e[2J", 14/*length*/);
        // backcolor(back);
        forecolor((inc::tty_color_t)color.fore);
        print_flush();
    }

    void cursor_visiable(bool value){
        the_cursor_visiable = true;
        print_core(value ? "\e[?25h" : "\e[?25l", 6/*length*/);
        print_flush();
    }

    void configure(bool echo){
        static bool is_echo         = false;
        static bool is_inited       = false;

        if (uxx echo_mask = echo ? 0 : ECHO; not is_inited or (is_echo != echo)){
            is_inited               = true;
            is_echo                 = echo;
            termios oldattr;
            tcgetattr(STDIN_FILENO, & oldattr);
            oldattr.c_lflag        &= ~(ICANON | echo_mask | ECHOE | ISIG);
            oldattr.c_cc[VSTART]    = 0xff;
            oldattr.c_cc[VSTOP]     = 0xff;
            tcsetattr(STDIN_FILENO, TCSANOW, & oldattr);
        }
    }

    inc::tty_key read_key(bool echo){
        inc::tty_key key;
        configure(echo);

        if (key_str.length() == 0){
            // 阻塞式读取
            buf_key[0]  = (char16_t)getchar();

            // 非阻塞式读取
            auto oldf   = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

            for(rest = 1; rest < sizeof(buf_key) / sizeof(buf_key[0]); rest++){
                if (auto v = getchar(); v == -1){
                    break;
                }
                else{
                    buf_key[rest] = v;
                }
            }

            fcntl(STDIN_FILENO, F_SETFL, oldf);
            key_str     = { buf_key, rest };
        }

        key             = decode(key_str, & rest);
        key_str         = key_str.backward(key_str.length() - rest);
        return key;
    }

    void print_core(asciis str, uxx length) {
        fwrite(str, 1, length, stdout);
    }

    #elif xis_windows

    inline HANDLE h_stdout;
    inline HANDLE h_stdin;

    xinit(inc::the_io_private_tty){
        CONSOLE_SCREEN_BUFFER_INFO info{};
        CONSOLE_CURSOR_INFO cursor_info;
        h_stdout        = (GetStdHandle(STD_OUTPUT_HANDLE));
        h_stdin         = (GetStdHandle(STD_INPUT_HANDLE));
        GetConsoleScreenBufferInfo(h_stdout, & info);
        GetConsoleCursorInfo(h_stdout, & cursor_info);
        color.fore      = (info.wAttributes >> 0) & 0xf;
        color.back      = (info.wAttributes >> 4) & 0xf;
        cursor_width    = (cursor_info.dwSize);
    };

    void print_core(asciis str, uxx length) {
        WriteConsoleA(h_stdout, str, DWORD(length), nullptr, nullptr);
    }

    inc::tty_key read_key(bool echo){
        // need lock ===================================
        inc::tty_key key;

        if (key_str.length() == 0) {
            DWORD          mode;
            DWORD          length;
            GetConsoleMode(h_stdin, & mode);
            SetConsoleMode(h_stdin, ~(
                ENABLE_LINE_INPUT | 
                ENABLE_PROCESSED_INPUT | (
                    not echo ? ENABLE_ECHO_INPUT : 0
                )
            ));
            ReadConsoleW(h_stdin, buf_key, sizeof(buf_key) / sizeof(buf_key[0]), & length, NULL);
            SetConsoleMode(h_stdin, mode); // recover
            key_str = { buf_key, length };
        }

        key                     = decode(key_str, & rest);
        key_str                 = key_str.backward(key_str.length() - rest);
        return key;
    }

    constexpr u08 map[]{
        u08(0),
        u08(FOREGROUND_RED),
        u08(FOREGROUND_GREEN),
        u08(FOREGROUND_RED | FOREGROUND_GREEN),
        u08(FOREGROUND_BLUE),
        u08(FOREGROUND_RED | FOREGROUND_BLUE),
        u08(FOREGROUND_GREEN | FOREGROUND_BLUE),
        u08(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE),
        u08(FOREGROUND_INTENSITY),

        u08(FOREGROUND_INTENSITY | FOREGROUND_RED),
        u08(FOREGROUND_INTENSITY | FOREGROUND_GREEN),
        u08(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN),
        u08(FOREGROUND_INTENSITY | FOREGROUND_BLUE),
        u08(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE),
        u08(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE),
        u08(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE),
    };

    void forecolor(inc::tty_color_t value) {
        color.fore          = map[uxx(value)];
        SetConsoleTextAttribute(h_stdout, color);
    }

    void backcolor(inc::tty_color_t value) {
        color.back          = map[uxx(value)];
        SetConsoleTextAttribute(h_stdout, color);
    }

    void clear(){
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        COORD coordScreen { 0, 0 };
        DWORD cCharsWritten;
        DWORD dwConSize;

        if (not GetConsoleScreenBufferInfo(h_stdout, & csbi)){
            return;
        }
        if (dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
            not FillConsoleOutputCharacter(h_stdout, TCHAR(' '), dwConSize, coordScreen, & cCharsWritten)){
            return;
        }
        if (not GetConsoleScreenBufferInfo(h_stdout, & csbi)){
            return;
        }
        if (not FillConsoleOutputAttribute(h_stdout, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)){
            return;
        }

        SetConsoleCursorPosition(h_stdout, coordScreen);
    }

    void cursor_visiable(bool value){
        CONSOLE_CURSOR_INFO info;
        info.bVisible       = value;
        info.dwSize         = DWORD(cursor_width);
        SetConsoleCursorInfo(h_stdout, & info);
    }

    #endif

    template<class item_t>
    inc::cxx<item_t> read_line(inc::ialloc<item_t> alloc){
        enum { initial_length = 128 };
        char    stack_buffer[initial_length];
        char *  buffer = stack_buffer;
        uxx     buffer_length = initial_length;
        uxx     total_length = 0;
        uxx     read_length = 0;
        uxx     dummy;

        auto inner_alloc = [](uxx bytes){
            return (char *)inc::memory::alloc<u08>(inc::memory::size{bytes});
        };
        auto inner_free = [&](voidp ptr, uxx bytes){
            if (stack_buffer == ptr){
                return;
            }
            inc::memory::free(ptr, inc::memory::size{bytes});
        };

        #if xis_linux || xis_mac
            auto h_stdin = fileno(stdin);
        #else
            h_stdin = (GetStdHandle(STD_INPUT_HANDLE));
        #endif

        do {
            auto buffer_rest = buffer_length - total_length;
            auto buffer_current = buffer + total_length;

            #if xis_linux || xis_mac
                if (read_length = read(h_stdin, buffer_current, buffer_rest);
                    buffer_current[read_length - 1] == '\n'){
                    read_length -= 1;
                }
                if (read_length > 0 and buffer_current[read_length - 1] == '\r'){
                    read_length -= 1;
                }
            #elif xis_windows 
                if (ReadConsoleA(h_stdin, buffer_current, DWORD(buffer_rest), LPDWORD(xref(read_length)), NULL);
                    buffer_current[read_length - 1] == '\n'){
                    read_length -= read_length > 1 ? 2 : 1;
                }
                else if (buffer_current[read_length - 1] == '\r'){
                    read_length -= 1;
                    ReadConsoleA(h_stdin, & dummy, 1, NULL, NULL);
                }
            #else
                #error "wait impliment"
            #endif

            if (total_length += read_length; buffer_length != total_length){
                break;
            }

            auto buffer_new = inner_alloc(buffer_length *= 2);
            inc::copy(buffer_new, buffer, total_length);
            inner_free(buffer, total_length);
            buffer = buffer_new;
        }while(true);

        auto target = inc::c08{ buffer, total_length }.template encode_item_as<item_t>(alloc);
        inner_free(buffer, buffer_length);
        return target;
    }

    inc::c08 read_line(inc::ialloc<char> alloc){
        return read_line<char>(alloc);
    }

    inc::c16 read_line(inc::ialloc<char16_t> alloc){
        return read_line<char16_t>(alloc);
    }
}
