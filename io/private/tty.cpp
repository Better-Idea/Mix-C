/*
问题：
- ctrl+m 会被映射成 ctrl+j
- 部分 ctrl+字母 和 ascii 的转义符重合，比如 \a \b \t \v \n
*/

#define xuser mixc::io_private_tty
#include"algo/binary_search.hpp"
#include"configure.hpp"
#include"docker/array.hpp"
#include"io/private/tty.hpp"
#include"io/private/tty_key_t.hpp"
#include"lang/cxx/compare.hpp"
#include"lang/cxx.hpp"
#include"macro/xdebug_fail.hpp"
#include<stdio.h>

namespace mixc::io_private_tty{
    print_t const print_core = & printf;

    void print_flush(){
        fflush(stdout);
    }

    #if xis_linux
    #include<fcntl.h>
    #include<termios.h>
    #include<unistd.h>
    using namespace inc::func_key;
    using namespace io_private;

    struct pair{ u08 key; u08 modify; inc::c08 value; };
    
    #define xgen(key,val,modify) pair{ key, modify, val) }
    static const & map = inc::array_view {
        xgen(esc,       "\x1b",                      is_func),
        xgen(f1,        "\x1b\x4f\x50",              is_func),
        xgen(f2,        "\x1b\x4f\x51",              is_func),
        xgen(f3,        "\x1b\x4f\x52",              is_func),
        xgen(f4,        "\x1b\x4f\x53",              is_func),
        xgen(f5,        "\x1b\x5b\x31\x35\x7e",      is_func),
        xgen(f6,        "\x1b\x5b\x31\x37\x7e",      is_func),
        xgen(f7,        "\x1b\x5b\x31\x38\x7e",      is_func),
        xgen(f8,        "\x1b\x5b\x31\x39\x7e",      is_func),
        xgen(top,       "\x1b\x5b\x31\x3b\x33\x41",  is_func | has_alt),
        xgen(buttom,    "\x1b\x5b\x31\x3b\x33\x42",  is_func | has_alt),
        xgen(right,     "\x1b\x5b\x31\x3b\x33\x43",  is_func | has_alt),
        xgen(left,      "\x1b\x5b\x31\x3b\x33\x44",  is_func | has_alt),
        xgen(end,       "\x1b\x5b\x31\x3b\x33\x46",  is_func | has_alt),
        xgen(home,      "\x1b\x5b\x31\x3b\x33\x48",  is_func | has_alt),
        xgen(f9,        "\x1b\x5b\x32\x30\x7e",      is_func),
        xgen(ins,       "\x1b\x5b\x32\x3b\x33\x7e",  is_func | has_alt),
        xgen(ins,       "\x1b\x5b\x32\x7e",          is_func),
        xgen(del,       "\x1b\x5b\x33\x3b\x33\x7e",  is_func | has_alt),
        xgen(del,       "\x1b\x5b\x33\x7e",          is_func),
        xgen(page_up,   "\x1b\x5b\x35\x7e",          is_func),
        xgen(page_down, "\x1b\x5b\x36\x7e",          is_func),
        xgen(top,       "\x1b\x5b\x41",              is_func),
        xgen(buttom,    "\x1b\x5b\x42",              is_func),
        xgen(right,     "\x1b\x5b\x43",              is_func),
        xgen(left,      "\x1b\x5b\x44",              is_func),
        xgen(end,       "\x1b\x5b\x46",              is_func),
        xgen(home,      "\x1b\x5b\x48",              is_func),
        xgen(backspace, "\x7f",                      is_func),
    };

    #undef xgen

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

    inc::tty_key_t read_key(){
        constexpr uxx buf_size = 16;
        constexpr uxx max_ctrl = 'z' - 'a' + 2;
        u08           buf[buf_size];
        auto          key = inc::tty_key_t().length(1);
        auto          ptr = buf;
        auto          end = buf + buf_size;
        uxx           len;

        auto && hint = [&](){
            ptr = buf;
            while(ptr < buf + len){
                print("\\x%x", ptr[0]);
                ptr += 1;
            }
        };

        while(true){
            ptr[0] = getchar(); // 阻塞读取
            ptr   += 1;

            while(ptr < end){
                if (auto v = hit(); v == not_exist){ // 非阻塞读取
                    break;
                }
                else{
                    ptr[0] = u08(v);
                    ptr   += 1;
                }
            }

            if (len = ptr - buf; len == 1){
                if (' ' <= buf[0] and buf[0] <= '~' or buf[0] == '\n'){ // 是 ascii
                    return key.is_ascii(true).value(buf[0]);
                }
                if (buf[0] < max_ctrl){ // 是 ctrl + ascii
                    return key.has_ctrl(true).is_ascii(true).value(buf[0] + 'a' - 1);
                }
            }

            if (auto c = buf[1]; len == 2 and buf[0] == 0x1b){
                if (' ' <= c and c <= '~'){ // alt + ascii
                    return key.has_alt(true).is_ascii(true).value(c);
                }
                if (c == 0x7f){ // alt + backspace
                    return key.has_alt(true).is_func(true).value(backspace);
                }
                else{
                    hint(); continue;
                }
            }

            pair item;
            uxx  index;
            item.value = inc::c08(buf, len);
            index      = inc::binary_search<pair>::match(map, item, []xcmp(pair){
                return left.value.compare(right.value);
            });

            if (index == not_exist){
                if (buf[0] == 0x1b or len > key.max_length){
                    hint(); continue;
                }

                key.length(len);

                for(uxx i = 0; i < len; i++){
                    key[i] = buf[i];
                }
            }

            auto & v = map[index];
            return key.has_ctrl((v.modify & has_ctrl) != 0)
                .has_alt((v.modify & has_alt) != 0)
                .is_func((v.modify & is_func) != 0)
                .value(v.key);
        }
    }
    #elif xis_windows
    // 临时设置
    inc::tty_key_t read_key(){
        return inc::tty_key_t().length(1).is_ascii(true).ascii(getchar());
    }
    #endif
}
