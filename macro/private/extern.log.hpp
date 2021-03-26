#undef  xuser
#define xuser mixc::macro_private_log::inc
#include"concurrency/lock/mutex.hpp"
#include"configure/switch.hpp"
#include"macro/private/log.hpp"
#include"macro/xdebug_fail.hpp"
#include"macro/xdefer.hpp"
#include"io/tty.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx/strlize.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"

namespace mixc::macro_private_log::origin{
    inline inc::mutex print_mutex;

    void log_set_color(message_type_t type){
        using namespace inc;

        tty_color_t conf[4];
        conf[uxx(success)] = tty_color_t::green;
        conf[uxx(normal )] = tty_color_t::light_gray;
        conf[uxx(warning)] = tty_color_t::yellow;
        conf[uxx(fail   )] = tty_color_t::red;
        tty.forecolor(conf[uxx(type)]);
    }

    void log_header_lock_free(log_type_t log_type, asciis file, uxx line, asciis func_name){
        using namespace inc;
        using namespace inc::ph;
        uxx skip = 0;

        // skip 值可能不是固定的
        #if xuse_xdebug_short_path
            if (auto i = c08{file}.index_of_last(xmixc); i != not_exist){
                skip  = i + sizeof(xmixc);
                skip += file[skip] == '/' or file[skip] == '\\';
            }
        #endif

        struct classify{
            asciis      type;
            tty_color_t color;
        };

        classify conf[4];
        conf[uxx(for_debug)] = { "DBUG", tty_color_t::light_gray};
        conf[uxx(for_fail )] = { "FAIL", tty_color_t::red};
        conf[uxx(for_test )] = { "TEST", tty_color_t::blue};
        conf[uxx(for_hint )] = { "HINT", tty_color_t::gray};

        auto i  = uxx(log_type);
        file   += skip;
        tty.forecolor(conf[i].color);
        tty.write(conf[i].type, " | ", v{file, ':', line}.l(50), " | ", v{func_name}.l(20), " | ");
    }

    void log(
        log_type_t          log_type, 
        asciis              file, 
        uxx                 line, 
        asciis              func_name, 
        asciis              message, 
        message_type_t      message_type
    ){
        using namespace inc;

        print_mutex.lock([&](){
            auto color = tty.forecolor();
            log_header_lock_free(log_type, file, line, func_name);
            log_set_color(message_type);
            tty.write_line(message);
            tty.forecolor(color);
        });
    }

    void log_hint_lock_free(
        asciis              message, 
        message_type_t      message_type, 
        inc::mix const    * items, 
        uxx                 items_length
    ){
        using namespace inc;
        char buf[32];
        auto backup = tty.forecolor();
        xdefer{
            tty.forecolor(backup);
        };
        log_set_color(message_type);

        for(uxx i = 0; i < items_length; i++, items++){
            // quotes_first 和 label_first 只有一个为 true
            // 指示当前元素是否是纯字符串
            bool quotes_first           = false;
            bool label_first            = false;
            bool in_origin_text         = false;
            uxx  brackets               = 0;

            do {
                if (message += 1; not in_origin_text){
                    switch(message[-1]){
                    case '(':  brackets++;  break;
                    case ')':  brackets--;  break;
                    case '\"': 
                        quotes_first    = not label_first; 
                        in_origin_text  = true;
                        break;
                    default:
                        label_first     = label_first or (message[-1] != ' ' and not quotes_first);
                        break;
                    }

                    tty.write(message[-1]);
                }
                else switch(message[-1]){
                case '\\':
                    tty.write("\\", message[0]);
                    message            += 1;
                    break;
                case '\"':
                    tty.write("\"");
                    in_origin_text      = false;
                    break;
                default:
                    tty.write(message[-1]);
                    break;
                }

                // message 表达式可能包含函数调用，小括号间包含个 ',' 分隔符
                // 例如：message = "a, b, c.call(a, \"hello\", c),"
                // 但需要下列表达式同时满足时才表示结束语义
            }while(message[0] != ',' or brackets != 0 or in_origin_text);

            if (message += 1; quotes_first){
                continue;
            }

            switch(items->fmt){
            case classify_type_t::is_char_t:        tty.write(':', items->c); break;
            case classify_type_t::is_ptr_t:         tty.write(':', items->v); break;
            case classify_type_t::is_signed_t:      tty.write(':', items->i); break;
            case classify_type_t::is_unsigned_t:    tty.write(':', items->u); break;
            case classify_type_t::is_float_t:
                tty.write(':', c08{items->f, float_format_t::fmt_s1p2Es3, 17/*precious*/, [&](uxx length){
                        return buf;
                    }
                });
                break;
            case classify_type_t::is_str_t:
                tty.write(':', 
                    "\"", 
                    items->slen == not_exist ? 
                        inc::c08{items->s} : 
                        inc::c08{items->s, items->slen}, 
                    "\""
                );
                break;
            default: 
                xdebug_fail("classify_type_t miss match");
                break;
            }
        }
    }

    void log(
        log_type_t          log_type, 
        asciis              file, 
        uxx                 line, 
        asciis              func_name, 
        asciis              message, 
        message_type_t      message_type,
        inc::mix const    * items, 
        uxx                 items_length
    ){
        using namespace inc;

        print_mutex.lock([&](){
            auto color = tty.forecolor();
            log_header_lock_free(log_type, file, line, func_name);
            log_hint_lock_free(message, message_type, items, items_length);
            tty.write_line();
            tty.forecolor(color);
        });
    }
}
